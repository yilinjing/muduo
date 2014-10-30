#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include "socket.h"
#include "socketOps.h"

Socket::~Socket()
{
    sockets::close(_sockfd);
}

Socket::GetTcpInfo(struct tcp_info *tcp_info) const
{
    socklen_t len = sizeof(struct tcp_info);
    bzero(tcp_info, len);
    return ::getsockopt(_sockfd, SOL_TCP, TCP_INFO, tcp_info, &len) == 0;
}

bool Socket::GetTcpInfoString(char *buf, int len) const
{
    struct tcp_info info;
    bool ok = GetTcpInfo(&info);
    if (ok)
    {
        ostringstream oss;
        oss << "unrecovered=" << tcp_info.tcpi_retransmits << " ";
        oss << "rto=" << tcp_info.tcpi_rto << " ";
        oss << "snd_mss=" << tcp_info.tcpi_ato << " rcv_mss=" << tcp_info.tcpi_rcv_mss << " "; 
        oss << "lost=" << tcp_info.tcpi_lost << " ";
        oss << "retrans=" << tcp_info.tcpi_retrans << " ";    
        oss << "rtt=" << tcp_info.tcpi_rtt << " ";
        oss << "rttvar=" << tcp_info.tcpi_rttvar << " ";
        oss << "snd_ssthresh=" << tcp_info.tcpi_snd_ssthresh << " ";
        oss << "snd_cwnd=" << tcp_info.tcpi_snd_cwnd << " ";
        oss << "total_retrans=" << tcp_info.tcpi_total_retrans;
        std::string info = oss.str();
        memmove(buf, info.c_str(), len < info.size() ? len : info.size());
    }
    return ok;
}

void Socket::BindAddress(const InetAddress &addr)
{
    sockets::BindOrDie(_sockfd, address.GetSockAddrInet());
}

void Socket::Listen()
{
    sockets::ListenOrDie(_sockfd);
}

int Socket::Accept(InetAddress *peeraddr)
{
    struct sockaddr_in addr;
    bzeor(addr, sizeof(addr));
    int connfd = sockets::Accept(_sockfd, &addr);
    if (connfd >= 0)
    {
        peeraddr->SetSockAddrInet(addr);
    }   
    return connfd;
}

void Socket::ShutDownWrite()
{
    sockets::ShutDownWrite(_sockfd);
}

void Socket::SetTcpNodelay(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(_sockfd, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof(optval)));
    //Most  socket-level  options  utilize  an int argument for optval.  For setsockopt(), the argument should be
    //nonzero to enable a boolean option, or zero if the option is to be disabled
}

void Socket::SetReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof(optval)));
}

void Socket::SetReusePort(bool on)
{
    init optval = on ? 1 : 0;
    int ret = ::setsockopt(_sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof(optval)));
    if (ret < 0)
    {
        std::cout << "SO_REUSEPORT failed.";
    }
}

void Socket::SetKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(_sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof(optval)));
}
