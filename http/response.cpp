#include "response.h"
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>

using namespace std;
using namespace ctk::http;

ctk::http::response::response()
{

}

void ctk::http::response::set_writer_callback( const http_rsp_writer_callback& wcb )
{
    wcb_ = wcb;
}

void ctk::http::response::set_status( int status )
{
    status_ = status;
}

void ctk::http::response::set_content_type( const std::string& ct )
{
    content_type_ = ct;
}

void ctk::http::response::add_header( const std::string& k, const std::string& v )
{
    headers_.push_back(str_pair(k, v));
}

void ctk::http::response::add_cookie( const cookie& c )
{
    cookies_.push_back(c);
}

void ctk::http::response::append_body( const std::string& buf )
{
    rsp_body_.append(buf);
}

int ctk::http::response::append_body_ex( const char* format, ... )
{
    if (NULL == format)
    {
        return -1;
    }

    va_list arglist;
    va_start(arglist, format);
    int est_len = vsnprintf(NULL, 0, format, arglist);
    if (est_len > 0)
    {
        char* buf = new char[est_len + 1];
        if (buf)
        {
            vsnprintf(buf, est_len, format, arglist);
            rsp_body_.append(buf, est_len);
            delete []buf;
        }

        return 0;
    }
    else
    {
        return -2;
    }
}

int ctk::http::response::write_all()
{
    // status code
    string head;
    head.reserve(1024);
    char buf[1024];
    int buf_str_len = 0;
    buf_str_len = snprintf(buf, sizeof(buf), "Status: %d\r\n", status_);
    head.append(buf, buf_str_len);
    // other headers
    for (str_pair_vec::iterator iter = headers_.begin(); iter != headers_.end(); ++iter)
    {
        head.append(iter->first).append(": ").append(iter->second).append("\r\n");
    }

    // set-cookie
    for (cookie_vec::iterator iter = cookies_.begin(); iter != cookies_.end(); ++iter)
    {
        head.append(iter->to_string()).append("\r\n");
    }

    // content type
    head.append("Content-Type: ").append(content_type_).append("\r\n");
    // content-length
    buf_str_len = snprintf(buf, sizeof(buf), "Content-Length: %" PRId64 "\r\n\r\n", (int64_t)rsp_body_.length());
    head.append(buf, buf_str_len);
    wcb_(head.c_str(), head.length());
    // body
    
    if (!rsp_body_.empty())
    {
        wcb_(rsp_body_.c_str(), rsp_body_.length());
    }

    return 0;
}

void ctk::http::response::prepare_output()
{
    status_ = 200;
    content_type_ = "text/html";
    content_length_ = 0;
    headers_.clear();
    cookies_.clear();
    rsp_body_.clear();
}
