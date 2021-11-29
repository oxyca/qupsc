#ifndef NETWORKTHREAD_H
#define NETWORKTHREAD_H

#include <QThread>
#include <QStringList>

namespace engine {

class NetworkThread : public QThread
{
    Q_OBJECT
public:
    explicit NetworkThread(QObject *parent = nullptr);
    bool isConnected() const;
public slots:
    bool start(const QString & host, quint16 port, quint16 pollingInterval);
    void stop();
    void refresh();
signals:
    void connecting();
    void connected();
    void disconnected();
    void waiting();
    void error(const QString & what);
    void deviceAdded(const QString & name);
    void deviceRemoved(const QString & name);
protected:
    void run() override;
private:
    QString _host;
    quint16 _port;
    quint16 _pollingInterval;
    bool _stopped = false;
    bool _reload = false;
};

}
#endif // NETWORKTHREAD_H
