#include <netdb.h>
#include <boost/static_assert.hpp>
#include "InetAddress.h"

InetAddress::InetAddress(uint32_t port, bool loopback)
{
    bzero(&_addr, sizeof(_addr));
    _addr.sin_family = AF_INET;
    in_addr_t ip = loopbackonly ? INADDR_ANY : INADDR_LOOPBACK;
    _addr.sin_addr.s_addr = sockets::HostToNetwork32(ip);
    _addr.sin_port = sockets::PortToNetWork32(port);
}

InetAddress::InetAddress(string ip, uint32_t port)
{
    bzeor(&_addr, sizeof(_addr));
    sockets::FromIpPort(ip.c_str(), port,,  &_addr);
}

string InetAddress::ToIpPort() const
{
    char buf[32];
    sockets::ToIpPort(buf, sizeof(buf), _addr);
    return buf;
}

string InetAddress::ToIp() const
{
    char buf[32];
    sockets::ToIp(buf, sizeof(buf), _addr);
}

bool InetAddress::Resolve(string hostname, InetAddress *out)
{
    struct hosent;
    struct hostent *he = NULL;
    int herrno = 0;
    bzero(&hent,0, sizeof(hosent));
    
    int ret = gethostbyname_r(hostname.c_str(), &hent, 
    struct hostent *he = NULL;
    int herrno = 0;
    bzero(&hent,0, sizeof(hosent));
    
    static char resolve_buf[64 * 1024];
    int ret = gethostbyname_r(hostname.c_str(), &hent, resolve_buf, sizeof(resolve_buf), &he, &errno);
    if (0 == ret && NULL !=he)
    {
        out->_addr.sin_addr = *reinterpret_cast<struct in_addr*>(he->h_addr);
        return true;
    }
    else
    {
        if (ret)
        {
            std::cout << "InetAddress::resolve failed.";
        }
        return false;
    }
}
