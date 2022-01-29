#include "networkthread.h"
#include <list>
#include <QDebug>

namespace engine {

NetworkThread::NetworkThread(QObject *parent) : QThread(parent)
{

}

bool NetworkThread::start(const QString &host, quint16 port, quint16 pollingInterval)
{
    if (isRunning()) return false;
    _stopped = false;
    _host = host;
    _port = port;
    _pollingInterval = pollingInterval;
    QThread::start();
    return true;
}

void NetworkThread::stop()
{
    _stopped = true;
}

void NetworkThread::refresh()
{
    _reload = true;
}

void NetworkThread::updateKnownDevices(nut::TcpClient & client, std::list<std::string> & knownDevices)
{
    auto devices = client.getDeviceNames();
    for (const auto &device: devices) {
        if (std::find(knownDevices.begin(), knownDevices.end(), device) == knownDevices.end()) {
            knownDevices.push_back(device);
            auto description = client.getDeviceDescription(device);
            emit deviceAdded(QString::fromLatin1(device.data(), device.size()),
                             QString::fromLatin1(description.data(), description.size()));
        }
    }
}

void NetworkThread::run()
{
    std::list<std::string> knownDevices;
    while (!_stopped) {
        _reload = false;
        nut::TcpClient client;
        try {
            emit connecting();
            client.connect(_host.toStdString(), _port);
            emit connected();
            emit waiting();
            updateKnownDevices(client, knownDevices);
            while (!_stopped && !_reload) {
                QThread::sleep(_pollingInterval);
            }
        } catch(nut::NutException & e) {
            emit error(QString::fromLatin1(e.str().data(), e.str().size()));
            qDebug() << QString::fromLatin1(e.str().data(), e.str().size());
        }
        catch(std::exception & e) {
            qDebug() << QString::fromLatin1(e.what());
        }
        catch(...) {
            qDebug() << "Unknown error";
        }
        client.disconnect();
        while (client.isConnected());
        emit disconnected();
    }
    deleteLater();
}

}
