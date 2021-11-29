#include "networkthread.h"
#include <nutclient.h>
#include <list>

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

void NetworkThread::run()
{
    std::list<std::string> knownDevices;
    while (!_stopped) {
        _reload = false;
        nut::TcpClient client(_host.toStdString(), _port);
        try {
            emit connecting();
            client.connect();
            emit connected();
            emit waiting();
            auto devices = client.getDeviceNames();
            for (const auto &device: devices) {
                if (std::find(knownDevices.begin(), knownDevices.end(), device) == knownDevices.end())
                    knownDevices.push_back(device);
                    emit deviceAdded(QString::fromLatin1(device.data(), device.size()));
            }
            while (!_stopped && !_reload) {
                QThread::sleep(_pollingInterval);
            }
        } catch(nut::NutException & e) {
            emit error(QString::fromLatin1(e.str().data(), e.str().size()));
        }
        client.disconnect();
        emit disconnected();
    }
    deleteLater();
}

}
