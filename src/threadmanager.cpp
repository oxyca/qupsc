#include "threadmanager.h"
#include "networkthread.h"
#include <QApplication>

namespace engine {

ThreadManager::ThreadManager(QObject *parent, uint16_t pollingInterval) : QObject(parent), m_pollingInterval(pollingInterval)
{

}

ThreadManager::~ThreadManager()
{
    stop();
}

void ThreadManager::newThread(const QString &address, uint16_t port)
{

    if (auto thi = m_threads.find(address); thi == m_threads.end()) {
        auto * thread = new NetworkThread();
        m_threads.insert(std::pair<QString, NetworkThread *>(address, thread));
        connect(thread, &NetworkThread::deleted, this, &ThreadManager::threadDeleted, Qt::QueuedConnection);
        connect(this, &ThreadManager::stopThreads, thread, &NetworkThread::stop, Qt::QueuedConnection);
        thread->start(address, port, m_pollingInterval);
    } else {
    //    _thi->
    }
}

void ThreadManager::stop()
{
    emit stopThreads();
    while (m_threads.size() > 0) {
        QApplication::processEvents();
    }
}

void ThreadManager::threadDeleted(const QString &host)
{
    if (m_threads.find(host) != m_threads.end()) {
        m_threads.erase(host);
        emit hostRemoved(host);
    }
}

}
