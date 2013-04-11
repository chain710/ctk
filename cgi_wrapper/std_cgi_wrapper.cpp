#include "std_cgi_wrapper.h"
#include "time_helper.h"
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


using namespace ctk;

enum
{
    fd_readable = 0x01, 
    fd_writable = 0x02, 
    fd_error = 0x04,
};

int ctk::std_cgi_wrapper::read_all_stdin( char *buf, size_t len, timeval timeout )
{
    // select stdin
    size_t rdlen = 0;
    timeval tv_beg, tv_end, tv_cost = {0, 0};
    gettimeofday(&tv_beg, NULL);
    while (rdlen < len && timeval_cmp(timeout, tv_cost) >= 0)
    {
        int ret = fd_wait(0, timeout);
        if (0 == ret)
        {
            // timeout
            // 土鳖做法，只有在超时的时候才加cost，避免频繁系统调用
            gettimeofday(&tv_end, NULL);
            timeval_add(tv_cost, timeval_diff(tv_end, tv_beg));
            continue;
        }
        else if (ret < 0)
        {
            return -1;
        }
        else if (ret & fd_readable)
        {
            ret = read(0, buf, len - rdlen);
            if (ret < 0)
            {
                if (EINTR == errno)
                {
                    continue;
                }
                else
                {
                    return -1;
                }
            }
        }
    }

    return rdlen;
}

int ctk::std_cgi_wrapper::read_param( const char* key, char* val, size_t val_len )
{
    if (key && val && val_len > 0)
    {
        char* v = getenv(key);
        if (v)
        {
            snprintf(val, val_len, "%s", v);
        }
        else
        {
            v[0] = '\0';
        }
    }
    
    return 0;
}

int ctk::std_cgi_wrapper::write_stdout( const char *buf, size_t len )
{
    return write(1, buf, len);
}

int ctk::std_cgi_wrapper::write_stderr( const char *buf, size_t len )
{
    return write(2, buf, len);
}

int ctk::std_cgi_wrapper::fd_wait( int fd, timeval timeout )
{
    fd_set rds, wrs, errs;
    FD_ZERO(&rds);
    FD_ZERO(&wrs);
    FD_ZERO(&errs);
    int ret = select(0 + 1, &rds, &wrs, &errs, &timeout);
    if (0 == ret)
    {
        // timeout
        return 0;
    }
    else if (ret < 0)
    {
        // select failed
        return -1;
    }
    else
    {
        ret = 0;
        if (FD_ISSET(0, &rds))
        {
            ret |= fd_readable;
        }
        if (FD_ISSET(0, &wrs))
        {
            ret |= fd_writable;
        }


        if (FD_ISSET(0, &errs))
        {
            ret |= fd_error;
        }
    }

    return ret;
}
