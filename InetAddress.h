#ifdef MUDUO_NET_INETADDRESS_H_
#define MUDUO_NET_INETADDRESS_H_

#include <netinet/in.h>
#include <boost/noncopyable.hpp>
#include <stdint.h>

class InetAddress : boost::noncopyable
{
public:
    explict InetAddress(uint32_t port = 0, bool loopbackonly = false);

    InetAddress(string ip, uint32_t port);

    InetAddress(const struct sockaddr_in &addr) : _addr(addr)
    {}

    string ToIp() const;
    string ToIpPort() const;

    const struct sockaddr_in & getSockAddrInet() const { return _addr;}
    void SetSockAddrInet(const struct sockaddr_in &addr) { _addr = addr;}
    
    uint32_t IpNetEndian() const { return _addr.sin_addr.s_addr; }
    uint32_t PortNetEndian() const { return _addr.sin_port; }

    static bool Resolve(string hostname, InetAddress *result);

private:
    struct sockaddr_in _addr;
};


#endif
