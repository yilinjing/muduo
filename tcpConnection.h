#ifndef MUDUO_NET_TCPCONNECTION_H
#define MUDUO_NET_TCPCONNECTION_H

#include <boost/any.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hppp>
#include <boost/shared_ptr.hpp>

#include <netinet/tcp.h>
#include <stdint.h>

struct tcp_info;
class Channel;
class EventLoop;
class Socket;

class TcpConnection : boost::noncopyable, 
    public boost::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *loop, 
                const string &name, 
                init sockfd, 
                const InetAddress &local_addr,
                const InetAddress &peer_addr;

    ~TcpConnection();

    EventLoop* GetLoop() const { return _loop;}
    const string& Name() const { return _name;}
    const InetAddress& LocalAddress() const { return _local_addr;}
    const InetAddress& PeerAddress() const { return _peer_addr;}
    bool Connected() const { _state = KConnected; }
    bool GetTcpInfo(struct tcp_info *) const;
    string GetTcpInfoString() const;

    void Send(const void *message, int len);
    void Send(const string& message);
    void Send(Buffer *message);
    
    void Shutdown();
    void ForceClose();
    void ForceCloseWithDelay(double seconds);
    void SetTcpNoDelay(bool on);
    
    void SetContext(const boost::any &context)
    { _context = context; }

    void boost::any& GetContext() const 
    { return &_context; }

    boost::any * GetMutableContext()    
    {   return &_context; }

    void SetConnectionCallback(const ConnectionCallback &cb)
    { _connection_callback = cb; }

    void SetMessageCallback(const MessageCallback &cb)
    {   _message_callback = cb; }
    
    void SetWriteCompleteCallback(const WriteCompleteCallback &cb)
    {   _writecomplete_callback = cb;}
    
    Buffer *InputBuffer() 
    {   _input_buffer;}

    Buffer *OutBuffer()
    {   _output_buffer;}
    
    void SetCloseCallback(const CloseCallBack cb)
    {   _close_callback = cb; }   
    
    void ConnectEstablished();
    void ConnectDestroyed();
    
private:
    enum StateE { KDisconnected, KConnecting, KConnected, KDisconnecting };
    void HandleRead(uint64_t recvtime);
    void HandleWrite();
    void HandlClose();
    void HandleError();
    void SetState(StateE s) { _state = s; }

    const string _name;
    StateE _state;
    
    boost::scoped_ptr<Socket> _socket;
    boost::scoped_ptr<Channel> _channel;
    const InetAddress _local_addr;
    const InetAddress _peer_addr;
    ConnectionCallback _connection_callback;
    MessageCallback _message_callback;
    WriteCompleteCallback _writecomplete_callback;
    CloseCallBack _close_callback;
    Buffer _input_buffer;
    Buffer _output_buffer;
    boost::any _context;
}

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
#endif
