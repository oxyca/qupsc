#include "nutsocket.h"
#include <QString>

namespace  Engine {

NutSocket::NutSocket()
{

}

NutSocket::~NutSocket()
{

}

void NutSocket::connect(const std::string &host, int port)
{
    _socket.connectToHost(QString::fromUtf8(host.data(), host.size()), port);
}

void NutSocket::disconnect()
{
    _socket.disconnect();
}

bool NutSocket::isConnected() const
{
    return _socket.state() == QAbstractSocket::ConnectedState;
}

size_t NutSocket::read(void *buf, size_t sz)
{
    return _socket.read(reinterpret_cast<char*>(buf), sz);
}

size_t NutSocket::write(const void *buf, size_t sz)
{
    return _socket.write(reinterpret_cast<const char*>(buf), sz);
}

std::string NutSocket::read()
{
    char buf[512];
    auto l = _socket.readLine(buf, 512);
    if (l != 0) {
        buf[l-1] = 0;
        return std::string(buf);
    }
    throw nut::IOException("Could not read line");
    return std::string();
}

void NutSocket::write(const std::string &s)
{
    auto vs = s + '\n';
    auto data = vs.data();
    size_t nextPos = 0;
    while (nextPos < vs.size()) {
        size_t bw = write(reinterpret_cast<const void *>(&data[nextPos]), vs.size() - nextPos);
        if (bw == 0)
            throw nut::IOException("Writing string failed");
        nextPos += bw;
    }
}

} // namespace Engine
