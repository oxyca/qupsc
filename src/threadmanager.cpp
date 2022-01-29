#include "threadmanager.h"
#include "networkthread.h"

namespace engine {

ThreadManager::ThreadManager(QObject *parent) : QObject(parent)
{

}

void ThreadManager::newThread(const QString &address, uint16_t port)
{
    auto _thi = _threads.find(address);
    _thi.operator ->()

    if (auto _thi = _threads.find(address); _thi == _threads.end()) {
        auto * thread = new NetworkThread();
        _threads.insert(std::pair<QString, NetworkThread *>(address, thread));
        thread->start(address, port, 4);
    } else {
        _thi->
    }
}

}
