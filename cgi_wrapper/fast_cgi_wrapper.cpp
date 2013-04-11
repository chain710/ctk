#include "fast_cgi_wrapper.h"
#include <stdio.h>

using namespace ctk;

int ctk::fast_cgi_wrapper::accept()
{
    return FCGX_Accept(&m_pInStream, &m_pOutStream, &m_pErrStream, &m_stEnv);
}

int ctk::fast_cgi_wrapper::close()
{
    FCGX_Finish();
    return 0;
}

int ctk::fast_cgi_wrapper::read_all_stdin( char *buf, size_t len, timeval timeout )
{
    if (buf && len > 0)
    {
        int rdlen = FCGX_GetStr(buf, len, m_pInStream);
        if (rdlen < 0)
        {
            return -1;
        }

        return rdlen;
    }

    return -1;
}

int ctk::fast_cgi_wrapper::read_param( const char* key, char* val, size_t val_len )
{
    if (NULL == key || NULL == val || val_len <= 0)
    {
        return -1;
    }

    const char* v = FCGX_GetParam(key, m_stEnv);
    if (v)
    {
        snprintf(val, val_len, "%s", v);
    }
    else
    {
        val[0] = '\0';
    }

    return 0;
}

int ctk::fast_cgi_wrapper::write_stdout( const char *buf, size_t len )
{
    return FCGX_PutStr(buf, len, m_pOutStream);
}

int ctk::fast_cgi_wrapper::write_stderr( const char *buf, size_t len )
{
    return FCGX_PutStr(buf, len, m_pErrStream);
}
