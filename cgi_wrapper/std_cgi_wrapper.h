/**
 * file: std_cgi_wrapper.h
 * author: marcuscai
 * date: 2011-9-5
 * desc: 
 */
#ifndef _CTK_STD_CGI_WRAPPER_H_
#define _CTK_STD_CGI_WRAPPER_H_

#include <sys/types.h>
#include <sys/time.h>

namespace ctk
{

    class std_cgi_wrapper
    {
    public:
        /**< 读取所有(len)的cgi输入,buf至少要提供len长 */
        int read_all_stdin(char *buf, size_t len, timeval timeout);
        int read_param(const char* key, char* val, size_t val_len);

        int write_stdout(const char *buf, size_t len);
        int write_stderr(const char *buf, size_t len);
    private:
        /**< 返回fd的读写状态, =0表示超时, <0表示错误 */
        int fd_wait(int fd, timeval timeout);
    };

#endif

}
