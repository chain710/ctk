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
        /**< ��ȡ����(len)��cgi����,buf����Ҫ�ṩlen�� */
        int read_all_stdin(char *buf, size_t len, timeval timeout);
        int read_param(const char* key, char* val, size_t val_len);

        int write_stdout(const char *buf, size_t len);
        int write_stderr(const char *buf, size_t len);
    private:
        /**< ����fd�Ķ�д״̬, =0��ʾ��ʱ, <0��ʾ���� */
        int fd_wait(int fd, timeval timeout);
    };

#endif

}
