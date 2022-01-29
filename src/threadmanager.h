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
    explicit ThreadManager(QObject *parent = nullptr);
    void newThread(const QString & address, uint16_t port);
signals:
private:
    std::map<QString, NetworkThread *> _threads;
};

}

#endif // THREADMANAGER_H
