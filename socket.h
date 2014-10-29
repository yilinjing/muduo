#ifndef MUDUO_NET_SOCKET_H
#define MUDUO_NET_SOCKET_H

#include <boost/noncopyable.hpp>
struct tcp_info;

class InetAddress;

class Socket : boost::noncopyable
{
public:
    explicit Socket(int sockfd) : _sockfd(sockfd) { }
    ~Socket();

    int Fd() const { return _sockfd;)
    
    bool GetTcpInfo(struct tcp_info *) const;
    bool GetTcpInfoString(char *buf, int len) const;

    void BindAddress(const InetAddress *localaddr);
    void Listen(); 
    
    int Accept(InetAddress *peeraddr);

    void ShutDownWriter();

    void SetTcpNoDelay(bool on);

    void SetReuseAddr(bool on);

    void SetReusePort(bool on);

    void SetKeepAlive(bool on);

private:
    const int _sockfd;
};

#endif
