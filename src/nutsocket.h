#ifndef NUTSSOCKET_H
#define NUTSSOCKET_H

#include <nutclient.h>
#include <QTcpSocket>
#include <memory>
#include <condition_variable>

namespace engine {

class NutSocket : public nut::AbstractSocket
{
public:
    NutSocket();
    ~NutSocket();
    void connect(const std::string& host, int port) override;
    void disconnect() override;
    void abort() override;
    bool isConnected() const override;
    size_t read(void* buf, size_t sz) override;
    size_t write(const void* buf, size_t sz) override;
    std::string read() override;
    void write(const std::string & s) override;
private:
    /*
     * This is a replacement for waitForReadyRead() which fails to worl reliably on some platforms.
     */
    void waitForBytesAvailable();
     QTcpSocket m_socket;
};

std::shared_ptr<nut::AbstractSocket> SocketFactory();

}
#endif // NUTSSOCKET_H
