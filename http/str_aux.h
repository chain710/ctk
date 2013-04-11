/**
 * file: str_aux.h
 * author: marcuscai
 * date: 2011-8-30
 * desc: http string aux
 */

#ifndef _CTK_HTTP_STR_AUX_H_
#define _CTK_HTTP_STR_AUX_H_

#include "form.h"
#include "cookie.h"

namespace ctk
{
namespace http
{
    // 解析cookie
    void parse_cookie(const std::string &inbuf, cookies_map& c);

    // 解析multipart的content
    void parse_multi_part(const std::string &inbuf, const std::string &type, multipart_map& m);

    // http://www.w3.org/Protocols/rfc1341/0_TableOfContents.html
    // Multipurpose Internet Mail Extensions
    // 解析MIME
    void parseMIME(const std::string &msg, multipart& mp);
    // 解析k=v&k=v的参数串
    // 内部会url_decode
    void parse_query_string(const std::string &inbuf, form_map &qs);

    std::string url_decode(const std::string& src);
    std::string url_decode_c(const char* src);
}
}
#endif
