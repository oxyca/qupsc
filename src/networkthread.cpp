#include "networkthread.h"
#include <list>
#include <QDebug>

namespace engine {

NetworkThread::NetworkThread(QObject *parent) : QThread(parent)
{

}

NetworkThread::~NetworkThread()
{
    emit deleted(_host);
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
    _host = host;
    _port = port;
    m_pollingInterval = pollingInterval;
    QThread::start();
    return true;
}

void NetworkThread::stop()
{
    std::lock_guard lk(m_m);
    m_stopped = true;
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

void NetworkThread::updateKnownDevices(std::list<std::string> & knownDevices)
{
    auto devices = m_client->getDeviceNames();
    for (const auto &device: devices) {
        if (std::find(knownDevices.begin(), knownDevices.end(), device) == knownDevices.end()) {
            knownDevices.push_back(device);
            auto description = m_client->getDeviceDescription(device);
            emit deviceAdded(QString::fromLatin1(device.data(), device.size()),
                             QString::fromLatin1(description.data(), description.size()));
        }
    }
}

using namespace std::chrono_literals;

void NetworkThread::run()
{
    std::list<std::string> knownDevices;
    while (!m_stopped) {
        m_client.reset();
        while (!m_stopped) {
            try {
                emit connecting();
                m_client->connect(_host.toStdString(), _port);
                emit connected();
                emit waiting();
                m_reload = false;
                updateKnownDevices(knownDevices);
                emit waiting();

                emit idle();
                do {
                    std::unique_lock lk(m_m);
                    if (m_stopped || m_reload)
                        break;
                    m_cv.wait_for(lk, m_pollingInterval*1000ms, [this]{return m_stopped || m_reload;});
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
        }
        if (m_client->isConnected())
            m_client->disconnect();
        emit disconnected();
    }
    deleteLater();
}

}
