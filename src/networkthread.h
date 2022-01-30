#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <memory>
#include <condition_variable>
#include <nutclient.h>
#include <QThread>
#include <QStringList>

namespace engine {

class NetworkThread : public QThread
{
    Q_OBJECT
public:
    explicit NetworkThread(QObject *parent = nullptr);
    ~NetworkThread();
    bool isConnected() const;
public slots:
    bool start(const QString & host, quint16 port, quint16 pollingInterval);
    void stop();
    void refresh();
    void pause();
    void abort();
signals:
    void connecting();
    void connected();
    void disconnected();
    void waiting();
    void idle();
    void paused(bool b);
    void error(const QString & what);
    void deviceAdded(const QString & name, const QString & description);
    void deviceRemoved(const QString & name);
    void deleted(const QString & host);
protected:
    void run() override;
private:
    void updateKnownDevices(std::list<std::string> & knownDevices);
    QString _host;
    quint16 _port;
    quint16 m_pollingInterval;
    bool m_stopped = false;
    bool m_paused = false;
    bool m_reload = false;
    std::shared_ptr<nut::TcpClient> m_client;
    std::mutex m_m;
    std::condition_variable m_cv;
};

}
#endif // NETWORKTHREAD_H
