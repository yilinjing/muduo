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
        
    }
}
