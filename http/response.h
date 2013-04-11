/**
 * file: response.h
 * author: marcuscai
 * date: 2011-9-2
 * desc: 常用功能封装
 */

#ifndef _CTK_HTTP_RESPONSE_H_
#define _CTK_HTTP_RESPONSE_H_

#include "cookie.h"
#include <vector>
#include <string>
#include <utility>
#include <tr1/functional>

namespace ctk
{
namespace http
{
    typedef std::tr1::function<int (const char*, size_t)> http_rsp_writer_callback;
    typedef std::pair<std::string, std::string> str_pair;
    typedef std::vector<str_pair> str_pair_vec;
    typedef std::vector<cookie> cookie_vec;

    class response
    {
    public:
        response();
        void prepare_output();
        void set_writer_callback(const http_rsp_writer_callback& wcb);

        // http content
        void set_status(int status);
        void set_content_type(const std::string& ct);
        //void set_content_length(size_t l);
        void add_header(const std::string& k, const std::string& v);
        void add_cookie(const cookie& c);
        void append_body(const std::string& buf);
        int append_body_ex(const char* format, ...);
        // TODO:想办法增加zero-copy的版本

        int write_all(); // write all data and clear cache
    private:
        int status_;    // http状态码
        std::string content_type_;
        size_t content_length_;
        str_pair_vec headers_;
        cookie_vec cookies_;
        std::string rsp_body_;

        http_rsp_writer_callback wcb_;
    };
}
}
#endif
