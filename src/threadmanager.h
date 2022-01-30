#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <memory>
#include <map>

namespace engine {

class NetworkThread;
class ThreadManager : public QObject
{
    Q_OBJECT
public:
    ThreadManager(QObject *parent, uint16_t pollingInterval);
    ~ThreadManager();
    void newThread(const QString & address, uint16_t port);
    void stop();
signals:
    void hostRemoved(const QString & address);
    void stopThreads();
private slots:
    void threadDeleted(const QString & host);
private:
    std::map<QString, NetworkThread *> m_threads;
    uint16_t m_pollingInterval;
};

}

#endif // THREADMANAGER_H
