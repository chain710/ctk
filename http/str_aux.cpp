#include "str_aux.h"
#include "string_helper.h"
#include <string.h>
#include <set>

using namespace ctk;
using namespace ctk::http;
using namespace std;

//内部函数, 定义

static const string BOUNDARY("boundary=");

enum
{
    e_key = 1,
    e_value,
};

static char hexToChar(char first, char second)
{
    int digit;

    digit = (first >= 'A' ? ((first & 0xDF) - 'A') + 10 : (first - '0'));
    digit *= 16;
    digit += (second >= 'A' ? ((second & 0xDF) - 'A') + 10 : (second - '0'));
    return static_cast<char>(digit);
}

static void cookie_append(cookies_map& c, string& k, string& v)
{
    trim(k);
    trim(v);
    if (!k.empty())
    {
        cookie &a = c[k];
        a.set_name(k);
        a.set_value(v);
    }
}

static void form_map_append(form_map& fm, const string& k, const string& v)
{
    if (!k.empty())
    {
        fm[url_decode(k)].push_back(url_decode(v));
    }
}

template<typename container, typename Pr>
void parse_string_pair(const string& src, std::set<char> delim, std::set<char> assign, container &c, Pr func)
{
    // split querystring by &
    string k, v;
    k.reserve(64);
    v.reserve(128);
    string empty_string;

    int status = e_key;
    for (string::const_iterator iter = src.begin(); iter != src.end(); ++iter)
    {
        if (delim.find(*iter) != delim.end())
        {
            switch (status)
            {
            case e_value:
                // normal
                func(c, k, v);
                break;
            case e_key:
                // assume value is empty
                if (!k.empty())
                {
                    func(c, k, empty_string);
                }

                break;
            }

            k.clear();
            v.clear();
            status = e_key;
        }
        else if (assign.find(*iter) != assign.end())
        {
            switch (status)
            {
            case e_value:
                // value contains '=', shouldn't be, but...
                v.append(1, *iter);
                break;
            case e_key:
                // normal
                if (k.empty())
                {
                    // fuck it, loop till find delim
                    while ((++iter) != src.end())
                    {
                        if (delim.find(*iter) == delim.end())
                        {
                            // move to the pos after delim
                            ++iter;
                            status = e_value;
                            break;
                        }
                    }
                }
                else
                {
                    status = e_value;
                }

                break;
            }
        }
        else
        {
            switch (status)
            {
            case e_value:
                v.append(1, (char)*iter);
                break;
            case e_key:
                k.append(1, *iter);
                break;
            }
        }
    }

    func(c, k, v);
}

template<typename container, typename Pr>
void parse_string_pair(const string& src, char delim, char assign, container &c, Pr func)
{
    set<char> delims;
    set<char> assigns;

    delims.insert(delim);
    assigns.insert(assign);
    parse_string_pair(src, delims, assigns, c, func);
}

static content_transfer_encoding get_encode(const char* encode)
{
    if (NULL == encode)
    {
        return cte_unknown;
    }

    if (0 == strcasecmp(encode, "7bit"))
    {
        return cte_7bit;
    }
    else if (0 == strcasecmp(encode, "8bit"))
    {
        return cte_8bit;
    }
    else if (0 == strcasecmp(encode, "binary"))
    {
        return cte_binary;
    }
    else if (0 == strcasecmp(encode, "quoted-printable"))
    {
        return cte_quoted_printable;
    }
    else if (0 == strcasecmp(encode, "base64"))
    {
        return cte_base64;
    }
    else if (0 == strcasecmp(encode, "ietf-token"))
    {
        return cte_ietf_token;
    }
    else if (0 == strncasecmp(encode, "x-", 2))
    {
        return cte_x_token;
    }

    // 其他都认为是默认的7bit编码
    return cte_7bit;
}

//----------------------------------

void ctk::http::parse_cookie( const std::string &inbuf, cookies_map& c )
{
    set<char> delims;
    set<char> assigns;

    delims.insert(';');
    delims.insert(',');
    assigns.insert('=');
    parse_string_pair(inbuf, delims, assigns, c, cookie_append);
}

void ctk::http::parse_query_string( const std::string &inbuf, form_map &qs )
{
    parse_string_pair(inbuf, '&', '=', qs, form_map_append);
}

std::string ctk::http::url_decode( const std::string& src )
{
    string result;
    string::const_iterator iter;
    char c;

    result.reserve(src.length() * 2);

    for (iter = src.begin(); iter != src.end(); ++iter)
    {
        switch (*iter)
        {
        case '+':
            result.append(1, ' ');
            break;
        case '%':
            // Don't assume well-formed input
            if (std::distance(iter, src.end()) >= 2
                && std::isxdigit(*(iter + 1)) && std::isxdigit(*(iter + 2)))
            {
                c = *++iter;
                result.append(1, hexToChar(c, *++iter));
            }
            // Just pass the % through untouched
            else
            {
                result.append(1, '%');
            }
            break;

        default:
            result.append(1, *iter);
            break;
        }
    }

    return result;
}

std::string ctk::http::url_decode_c( const char* src )
{
    string src2;
    if (NULL == src)
    {
        return src2;
    }
    
    src2.assign(src);
    return url_decode(src2);
}

/*
 * http://www.w3.org/Protocols/rfc1341/7_2_Multipart.html
 * multipart格式如下
 * Content-Type: multipart/form-data; boundary=B\r\n
 * Content-Length: 160613\r\n\r\n
 * ignored content...
 * --B\r\n
 * Content-Disposition: form-data; name="N"; filename="FN"\r\n
 * Content-Type: CT\r\n\r\n
 * content....\r\n
 * --B\r\n
 * Content-Disposition: form-data; name="N"\r\n\r\n
 * value\r\n
 * --B\r\n
 * msg-head\r\n\r\n
 * msg-body\r\n
 * --B\r\n\r\n
 * msg-body\r\n
 * --B--
 * ignored content...
 */
void ctk::http::parse_multi_part( const std::string &inbuf, const std::string& type, multipart_map& m )
{
    string::size_type pos = type.find(BOUNDARY);

    string sep_base(type.substr(pos + BOUNDARY.length()));
    string sep_beg, sep_end;
    sep_beg.reserve(BOUNDARY.length()*2);
    sep_end.reserve(BOUNDARY.length()*2);

    sep_beg.append("--").append(sep_base).append("\r\n");
    sep_end.append("--").append(sep_base).append("--");

    string::size_type start = inbuf.find(sep_beg);
    string::size_type sepbeg_len = sep_beg.length();
    string::size_type old_pos = start + sepbeg_len;

    multipart mp;
    while (true)
    {
        mp.clear();
        // find next begin seperator

        pos = inbuf.find(sep_beg, old_pos);
        if (pos == string::npos)
        {
            break;
        }

        // parse data between 2 sep
        parseMIME(inbuf.substr(old_pos, pos - old_pos), mp);
        // if mp's name is empty, just ignore the content
        if (!mp.name.empty()) m[mp.name].push_back(mp);

        // update old pos
        old_pos = pos + sepbeg_len;
    }

    // find sep_end
    pos = inbuf.find(sep_end, old_pos);
    if (string::npos != pos)
    {
        mp.clear();
        parseMIME(inbuf.substr(old_pos, pos - old_pos), mp);
        if (!mp.name.empty()) m[mp.name].push_back(mp);
    }
}

void ctk::http::parseMIME( const std::string &msg, multipart& mp )
{
    // find header
    const static string head_sep = "\r\n\r\n";
    const static string nohead_sep = "\r\n";
    const static string content_disp = "Content-Disposition";
    const static string content_type = "Content-Type";
    const static string content_encode = "Content-Transfer-Encoding";
    const static string MIME_name = "name=\"";
    const static string MIME_filename = "filename=\"";
    const static string MIME_quote = "\"";
    string::size_type head_sep_pos = msg.find(head_sep);
    if (string::npos == head_sep_pos)
    {
        // 可能是没有头部的情况， 默认text/plain
        // 看看开头两个字符是否是\r\n
        if (0 == strncmp(nohead_sep.c_str(), msg.c_str(), nohead_sep.length()))
        {
            mp.type = "text/plain";
            head_sep_pos = nohead_sep.length();
        }
        else
        {
            // 非法的MIME消息, 不做处理
            return;
        }
    }
    else
    {
        // http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.2
        // Each header field consists of a name followed by a colon (":") and the field value. 
        // Field names are case-insensitive. The field value MAY be preceded by any amount of LWS, though a single SP is preferred.
        // Header fields can be extended over multiple lines by preceding each extra line with at least one SP or HT.

        string MIME_header = msg.substr(0, head_sep_pos + head_sep.length());
        string::size_type header_seek_off = 0;
        string n, v;
        // 这里循环的处理头部
        // 这里+2是头部标识结束的\r\n
        while (header_seek_off + 2 < MIME_header.length())
        {
            string::size_type delim = MIME_header.find(":", header_seek_off);
            if (string::npos != delim)
            {
                n = MIME_header.substr(header_seek_off, delim - header_seek_off);
                header_seek_off = delim + 1;    // 1==length of :
            }
            else
            {
                // 非法的格式, 但是继续处理
                n.clear();
            }

            string::size_type lb_pos = MIME_header.find("\r\n", header_seek_off);

            if (string::npos == lb_pos)
            {
                // 非法的格式, 抛弃
                return;
            }

            v = MIME_header.substr(header_seek_off, lb_pos - header_seek_off);
            trim(v);
            header_seek_off = lb_pos + 2;   // 2==length of \r\n

            if (0 == strncasecmp(content_disp.c_str(), n.c_str(), content_disp.length()))
            {
                // Content-Disposition
                mp.disposition = v;
                mp.name = extractBetween(mp.disposition, MIME_name, MIME_quote);
                mp.file = url_decode(extractBetween(mp.disposition, MIME_filename, MIME_quote));
                trim(mp.name);
                trim(mp.file);
            }
            else if (0 == strncasecmp(content_encode.c_str(), n.c_str(), content_encode.length()))
            {
                // Content-Transfer-Encoding
                mp.encode = get_encode(v.c_str());
            }
            else if (0 == strncasecmp(content_type.c_str(), n.c_str(), content_type.length()))
            {
                // Content-Type
                mp.type = v;
            }
            
        }

        head_sep_pos += head_sep.length();
    }

    // 剩下的是msg-body, -2是去掉尾巴上的\r\n(rfc定义)
    mp.content = msg.substr(head_sep_pos, msg.length() - head_sep_pos - 2);
}
