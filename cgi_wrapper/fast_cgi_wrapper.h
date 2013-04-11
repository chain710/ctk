/**
 * file: fast_cgi_wrapper.h
 * author: marcuscai
 * date: 2011-9-5
 * desc: 
 */
#ifndef _CTK_FAST_CGI_WRAPPER_H_
#define _CTK_FAST_CGI_WRAPPER_H_

#include <sys/types.h>
#include <sys/time.h>
#include <fcgiapp.h>

namespace ctk
{

    class fast_cgi_wrapper
    {
    public:
        int accept();
        int close();
        int read_all_stdin(char *buf, size_t len, timeval timeout);
        int read_param(const char* key, char* val, size_t val_len);

        int write_stdout(const char *buf, size_t len);
        int write_stderr(const char *buf, size_t len);
    private:
        FCGX_Stream*    m_pInStream;
        FCGX_Stream*    m_pOutStream;
        FCGX_Stream*    m_pErrStream;
        FCGX_ParamArray m_stEnv;
    };

}

#endif
