#include <errno.h>
#include <sys/uio.h>

#include "buffer.h"

const char Buffer::KCRLF[] = "\r\n";
const size_t Buffer::KCheapPrepend;
const size_t Buffer::KInitialSize;

ssize_t Buffer::ReadFd(int fd, int *save_errno)
{
    char *extrabuf[65536];
    struct iovec vec[2];
    const size_t writeable = WriteAbleBytes();
    vec[0].iov_base = Begin() + _write_index;
    vec[0].io_len = writeable;
    vev[1].iov_base = extrabuf;
    vec[1].io_len = sizeof(extrabuf);

    const int iovcnt = (writeable < sizeof(extrabuf) ? 2 : 1;
    if (n < 0)
    {
        *save_errno = errno;
    }
    else if(implicit_cast<size_t>(n) <= writeable)
    {
        _write_index += n;
    }
    else
    {
        _write_index = _buffer.size();
        Append(extrabuf, n - writeable);
    }
    return n;
}
