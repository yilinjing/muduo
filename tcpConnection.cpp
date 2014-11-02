#include <boost/bind.hpp>
#include <errno.h>
#include  "tcpConnection.h"

TcpConnection::TcpConnection(EventLoop *loop,
                            const string &name,
                            int sockfd,
                            const InetAddress localaddr,
                            const InetAddress peeraddr)
    : _loop(loop), _name(name), _state(KConnecting),
      _socket(new Socket(sockfd)), _channel(new Channel(loop, sockfd)),
      _local_addr(localaddr), _peer_addr(peeraddr)
{
    _channel->SetReadCallback(boost::bind(&TcpConnection::HandleRead, this, _1));
    _channel->SetWriteCallback(boost::bind(&TcpConnection::HandleWrite, this));
    _channel->SetCloseCallback(boost::bind(&TcpConnection::HandleClose, this));
    _channel->SetErrorCallback(boost::bind(&TcpConnection::HandleError, this));
    _socket->SetKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
}

bool TcpConnection::GetTcpInfo(struct tcp_info *tcpi) const
{
    return _socket->GetTcpInfo(tcpi);
}

string TcpConnection::GetTcpInfoString() const
{
    char buf[1024];
    buf[0] = '\0';
    _socket->GetTcpInfoString(buf, sizeof(buf));
    return buf;
}

void TcpConnection::Send(const void *data, int len)
{
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool fault_error = false;
    if (_state == KDisconnected)
    {
        std::cout << "disconnected, give up writing";
        return;
    }

    if (!_channel->IsWriting() && _output_buffer.ReadableBytes() == 0)
    {
        nwrote = sockets::write(_channel->Fd(), data, len);
        if (nwrote >= 0)
        {
            remaining = len - nwrote;
            if (remaining == 0 && 
        }
    }

    Send(string(static_cast<const char *>(data, len)));
}

void TcpConnection::Send(Buffer *buf)
{
    if(_state == KConnected)
    {   
        //pass;
    }
}

void TcpConnection::Shutdown()
{
    if(_state == KConnected)
    {
        if (!_channel->IsWriting())
        {
            _socket->ShutdownWrie();
        }
    }
}

void TcpConnection::ForceClose()
{
    if(_state == KConnected || _state == KDisconnecting)
    {
        SetState(KDisconnecting);
        HandlClose();
    }
}

void TcpConnection::ForceCloseWithDelay(double seconds)
{
    if(_state == kConnected || _state == KDisconnecting)
    {
        SetState(KDisconnecting);
        TcpConnection::ForceClose();
    }
}

void TcpConnection::SetTcpNoDelay(bool on)
{
    _socket->SetTcpNoDelay(on);
}

void TcpConnection::ConnectEstablished()
{
    SetState(KConnected);
    _channel->EnableReading();
    _connection_callback(shared_from_this());
}

void TcpConnection::ConnectDestroyed()
{
    if (_state == KConnected)
    {
        SetState(KDisconnected);
        _channel->DisableAll();

        _connection_callback(shared_from_this());
    }
    _channel->Remove();
}

void TcpConnection::HandleRead(uint64_t recv_time)
{
    int saved_errno = 0;
    ssize_t n = _input_buffer.ReadFd(_channel->Fd(), &saved_errno);
    if (n > 0)
    {
        _message_callback(shared_from_this(), &_input_buffer, recv_time);
    }
    else if (0 == n)
    {
        HandleClose();
    }
    else
    {
        errno = saved_errno;
        std::cout << "TcpConnection::HandleRead";
        HandleError();
    }
}

void TcpConnection::HandleWrite()
{
    if(_channle->IsWriting())
    {
        ssize_t n = sockets::write(_channel->Fd(), 
                                   _output_buffer.Peek(),
                                   _output_buffer.ReadableBytes());
        if (n > 0)
        {
            _output_buffer.Retrive(n);
            if (_output_buffer.ReadableBytes() == 0)
            {
                _channel->DisableWriting();
                if (_writecomplete_callback)
                {
                    boost::bind(_writecomplete_callback, shared_from_this());
                }
                if (_state == KDisconnecting)
                {
                    Shutdown();
                }
            }
            
        }
        else
        {
            std::cout << "TcpConnection::HandleWrite";
        }
    }
    else
    {
        std::cout << "Connection fd = " << _channel->Fd() 
                  << " is down, no more writing";
    }
}

void TcpConnection::HandleClose()
{
    SetState(KDisconnected);
    _channel->DisableAll();
}

void TcpConnection::HandleError()
{
    int err = sockets::GetSocketError(_channel->Fd());
    std::cout << "TcpConnection::HandleError [" << _name;
        << "] - SO_ERROR = " << err << " " << strerror_tl(err);
}
