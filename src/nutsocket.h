#ifndef NUTSSOCKET_H
#define NUTSSOCKET_H

#include <nutclient.h>
#include <QTcpSocket>

namespace Engine {

class NutSocket : public nut::AbstractSocket
{
public:
    NutSocket();
    ~NutSocket();
    void connect(const std::string& host, int port) override;
    void disconnect() override;
    bool isConnected() const override;
    size_t read(void* buf, size_t sz) override;
    size_t write(const void* buf, size_t sz) override;
    std::string read() override;
    void write(const std::string & s) override;
private:
     QTcpSocket _socket;
};

}
#endif // NUTSSOCKET_H
