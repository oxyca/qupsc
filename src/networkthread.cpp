#include "networkthread.h"
#include <list>
#include <set>
#include <QDebug>

namespace engine {

NetworkThread::NetworkThread(QObject *parent) : QThread(parent)
{

}

NetworkThread::~NetworkThread()
{
    emit deleted(m_host);
}

void NetworkThread::abort()
{
    m_client->abort();
    stop();
}

bool NetworkThread::start(const QString &host, quint16 port, quint16 pollingInterval)
{
    if (isRunning()) return false;
    m_stopped = false;
    m_host = host;
    m_port = port;
    m_pollingInterval = pollingInterval;
    QThread::start();
    return true;
}

void NetworkThread::stop()
{
    {std::lock_guard lk(m_m);
    m_stopped = true;}
    m_cv.notify_all();
}

void NetworkThread::refresh()
{
    std::lock_guard lk(m_m);
    m_reload = true;
    m_cv.notify_all();
}

void NetworkThread::pause()
{
    m_paused = !m_paused;
    emit paused(m_paused);
}

void NetworkThread::updateKnownDevices(std::list<Device> &knownDevices)
{
    auto devices = getDevices();
    std::list<Device> added_devs;
    std::list<Device> removed_devs;
    std::set_difference(devices.begin(), devices.end(), knownDevices.begin(), knownDevices.end(), std::inserter(added_devs, added_devs.begin()));
    std::set_difference(devices.begin(), devices.end(), knownDevices.begin(), knownDevices.end(), std::inserter(removed_devs, removed_devs.begin()));
    for (auto & device : added_devs) {
        emit deviceAdded(device.name, device.description);
        knownDevices.push_back(device);
        qDebug() << "device added: " << device.name;
    }
    for (const auto & device : removed_devs)
        emit deviceRemoved(device.name);
}

std::list<Device> NetworkThread::getDevices()
{
    std::list<Device> result;
    for (const auto & device : m_client->getDevices()) {
        Device d;
        d.name = QString::fromLatin1(device.getName().c_str());
        d.description = QString::fromLatin1(const_cast<nut::Device &>(device).getDescription().c_str());
        result.push_back(d);
    }
    return result;
}

using namespace std::chrono_literals;

void NetworkThread::run()
{
    std::list<Device> knownDevices;
    while (!m_stopped) {
        m_client.reset();
        m_client = std::shared_ptr<nut::TcpClient>(new nut::TcpClient());
        while (!m_stopped) {
            try {
                emit connecting();
                m_client->connect(m_host.toStdString(), m_port);
                emit connected();
                emit waiting();
                m_reload = false;
                updateKnownDevices(knownDevices);
                emit waiting();
                if (m_client->isConnected())
                    m_client->disconnect();
                emit idle();
                do {
                    std::unique_lock lk(m_m);
                    if (m_stopped || m_reload)
                        break;
                    m_cv.wait_for(lk, m_pollingInterval*1000ms, [this]{
                        return m_stopped || m_reload;

                    });
                } while (m_paused);
                if (m_paused)
                    emit paused(false);
                m_paused = false;
            } catch(nut::NutException & e) {
                emit error(QString::fromLatin1(e.str().data(), e.str().size()));
                qDebug() << QString::fromLatin1(e.str().data(), e.str().size());
                break;
            }
            catch(std::exception & e) {
                qDebug() << QString::fromLatin1(e.what());
                break;
            }
            catch(...) {
                qDebug() << "Unknown error";
                break;
            }
            if (m_client->isConnected())
                m_client->disconnect();
        }
        emit removeMe(m_host);
    }
 }

}
