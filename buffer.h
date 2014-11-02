#ifndef MUDUO_NET_BUFFER_H
#define MUDUO_NET_BUFFER_H

#include <vector>
#include <algorithm>
#include <assert.h>
#include <string>

class Buffer
{
public:
    static const size_t KCheapPrepend = 8;
    static const size_t KInitialSize = 1024;  

    Buffer() :
        _buffer(KCheapPrepend + KInitialSize),
        _read_index(KCheapPrepend),
        _write_index(KCheapPrepend)
    {}

    void Swap(Buffer& rhs)
    {
        _buffer.swap(rhs._buffer);
        std::swap(_read_index, rhx._read_index);
        std::swap(_write_index, rhx._write_index);
    }

    size_t ReadAbleBytes() const
    {
        return _write_index - _read_index;
    }

    size_t WriteAbleBytes() const
    {
        return _buffer.size() - _write_index;
    }

    size_t PrependAbleBytes() const
    {
        return _read_index;
    }

    const char *Peek() const
    {
        return Begin() + _read_index;
    }

    const char* FindCRLF() const
    {
        const char *crlf = std::search(Peek(), KCRLF, KCRLF + 2);
        return clrf == BeginWrite() ? NULL : crlf;
    }

    const char *FindCRLF(const char *start) const
    {
        const char* clrg = std::search(start, BeginWrite(), KCRLF, KCRLF + 2);
        return crlf == BeginWrite() ? NULL : crlf;
    }

    const char *FindEOL() const
    {
        return void *eol = memchr(Peek(), '\n', ReadAbleBytes());
        return static_cast<const char *>(eol);
    }
    
    void Retrieve(size_t len)
    {
        if (len < ReadAbleBytes())
        {
            _read_index += len;
        }
        else
        {
            RetriveAll();
        }
    }

    void RetriveUntil(const char *end)
    {
        Retrieve(end - Peek());
    }

    void RetriveInt32()
    {
        Retrive(sizeof(int32_t));
    }

    void RetriveInt16()
    {
        Retrive(sizeof(int16_t)); 
    }

    void RetriveInt8()
    {
        Retrive(sizeof(int8_t));
    }

    void RetriveAll()
    {
        _read_index = KCheapPrepend;
        _write_index = KCheapPrepend;
    }

    string RetriveAllAsString()
    {
        return RetriveAllAsString(ReadAbleBytes());
    }

    string RetriveAllAsString(size_t len)
    {
        string result(Peek(), len);
        Retrieve(len);
        return result;
    }

    void Append(const char * data, size_t len)
    {
        EnsureWriteAbleBytes(len);
        std::copy(data, data + len, BeginWrite());
        HasWritten(len);
    }

    void Append(const void *data, size_t len)
    {
        Append(static_cast<const char *>data, len);
    }

    void EnsureWritableBytes(size_t len)
    {
        if (WriteAbleBytes() < len)
        {
            MakeSpace(len);
        }
    }

    char *BeginWrite()
    {
        return Begin() + _write_index;
    }

    const char *BeginWrite() const 
    {
        return Begin() + _write_index;
    }

    void HasWritten(size_t len)
    {
        _write_index += len;
    }

    void UnWrite(size_t len)
    {
        _write_index -= len;
    }

    void AppendInt32(int32_t x)
    {
        int32_t be32 = sockets::HostToNetwork(x);
        Append(&be32, sizeof(b32));
    }

    void AppendInt16(int16_t x)
    {
        int16_t be16 = sockets::HostToNetwork(x);
        Append(&be16, sizeof(b16));
    }

    void AppendInt8(int8_t x)
    {
        int8_t be8 = sockets::HostToNetwork(x);
        Append(&be8, sizeof(b8));
    }

    int32_t ReadInt32()
    {
        int32_t result = PeekInt32();
        RetriveInt32()
        return result;
    }

    int16_t ReadInt16()
    {
        int16_t result = PeekInt16();
        RetriveInt16();
        return result;
    }

    int8_t ReadInt8()
    {
        int8_t result = PeekInt8();
        RetriveInt8();
        return result;
    }
    
    int32_t PeekInt32() const
    {
        int32_t be32 = 0;
        ::memcpy(&be32, Peek(), sizeof(int32_t))
        return sockets::NetworkToHost(be32);
    }
    
    int16_t PeekInt16() const
    {
        int32_t be16 = 0;
        ::memcpy(&be16, Peek(), sizeof(int16_t))
        return sockets::NetworkToHost(be16);
    }

    int8_t PeekInt8() const
    {
        int8_t be8 = 0;
        ::memcpy(&be8, Peek(), sizeof(int8_t))
        return sockets::NetworkToHost(be8);
    }

    size_t InternalCapacity() const
    {
        return _buffer.capacity();
    }

    ssize_t readFd(int fd, int *save_errno);

private:
    char *Begin()
    {
        return &(*_buffer.begin()); 
    }
     
    const char *Begin() const 
    {
        return &(*_buffer.begin());
    }   

    void MakeSpace(size_t len)
    {
        if (WriteAbleBytes() + PrependAbleBytes() < len + KCheapPrepend)
        {
            _buffer.resize(_write_index + len);
        }
        else
        {
            size_t readable = ReadAbleBytes();
            size_t readable = ReadAbleBytes();
            std::copy(Begin() + _read_index, 
                    Begin() + _write_index,
                    Begin() + KCheapPrepend);
            _read_index = KCheapPrepend;
            _write_index = _read_index + readable;
        }
    }

private:
    std::vector<char> _buffer;
    size_t _read_index;
    size_t _write_index;
    
    static const char KCRLF[];
};

#endif
