#include "threadmanager.h"
#include "networkthread.h"

namespace engine {

ThreadManager::ThreadManager(QObject *parent, uint16_t pollingInterval) : QObject(parent), m_pollingInterval(pollingInterval)
{

}

void ThreadManager::newThread(const QString &address, uint16_t port)
{

    if (auto thi = m_threads.find(address); thi == m_threads.end()) {
        auto * thread = new NetworkThread();
        m_threads.insert(std::pair<QString, NetworkThread *>(address, thread));
        connect(thread, &NetworkThread::deleted, this, &ThreadManager::threadDeleted, Qt::QueuedConnection);
        thread->start(address, port, m_pollingInterval);
    } else {
    //    _thi->
    }
}

void ThreadManager::threadDeleted(const QString &host)
{
    m_threads.erase(host);
    emit hostRemoved(host);
}

}
