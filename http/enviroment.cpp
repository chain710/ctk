#include "enviroment.h"
#include "str_aux.h"
#include <strings.h>
#include <stdlib.h>

using namespace ctk::http;
using namespace std;

enviroment::enviroment()
{
    server_port_ = 0;
    content_length_ = 0;
    is_https_ = false;
    method_ = hrm_unknown;
}

enviroment::enviroment( const enviroment& env )
:server_port_(env.get_server_port()), 
content_length_(env.get_content_length()), 
is_https_(env.is_https()), 
method_(env.get_method()), 
server_name_(env.get_server_name()),
path_info_(env.get_path_info()), 
path_translated_(env.get_path_translated()), 
script_name_(env.get_script_name()), 
query_string_(env.get_query_string()), 
remote_host_(env.get_remote_host()), 
remote_addr_(env.get_remote_addr()), 
auth_type_(env.get_auth_type()), 
content_type_(env.get_content_type()), 
user_agent_(env.get_user_agent()), 
accept_(env.get_accept()), 
referer_(env.get_referer()), 
cookies_(env.cookies_), 
reader_(env.reader_)
{

}

enviroment::enviroment( const env_reader& reader )
{
    set_env_reader(reader);
}

int enviroment::read_all()
{
    string tmp;
    reader_("SERVER_PORT", tmp);
    server_port_ = strtol(tmp.c_str(), NULL, 10);

    reader_("CONTENT_LENGTH", tmp);
    content_length_ = strtoul(tmp.c_str(), NULL, 10);

    reader_("HTTPS", tmp);
    is_https_ = (0 == strncasecmp(tmp.c_str(), "on", tmp.length()));

    reader_("REQUEST_METHOD", tmp);
    method_ = parse_method(tmp);

    reader_("SERVER_NAME", server_name_);
    reader_("PATH_INFO", path_info_);
    reader_("PATH_TRANSLATED", path_translated_);
    reader_("SCRIPT_NAME", script_name_);
    reader_("QUERY_STRING", query_string_);
    reader_("REMOTE_HOST", remote_host_);
    reader_("REMOTE_ADDR", remote_addr_);
    reader_("AUTH_TYPE", auth_type_);
    reader_("CONTENT_TYPE", content_type_);
    reader_("HTTP_USER_AGENT", user_agent_);
    reader_("HTTP_ACCEPT", accept_);
    reader_("HTTP_REFERER", referer_);

    parse_query_string(query_string_);
    reader_("HTTP_COOKIE", tmp);
    parse_cookie(tmp);

    return 0;
}

void enviroment::dump_all(std::string& out)
{
    string tmp;
    tmp.reserve(1024);
    reader_("SERVER_PORT", tmp);
    out.append("SERVER_PORT=").append(tmp);
    reader_("CONTENT_LENGTH", tmp);
    out.append(";CONTENT_LENGTH=").append(tmp);
    reader_("HTTPS", tmp);
    out.append(";HTTPS=").append(tmp);
    reader_("REQUEST_METHOD", tmp);
    out.append(";REQUEST_METHOD=").append(tmp);
    reader_("SERVER_NAME", tmp);
    out.append(";SERVER_NAME=").append(tmp);
    reader_("PATH_INFO", tmp);
    out.append(";PATH_INFO=").append(tmp);
    reader_("PATH_TRANSLATED", tmp);
    out.append(";PATH_TRANSLATED=").append(tmp);
    reader_("SCRIPT_NAME", tmp);
    out.append(";SCRIPT_NAME=").append(tmp);
    reader_("QUERY_STRING", tmp);
    out.append(";QUERY_STRING=").append(tmp);
    reader_("REMOTE_HOST", tmp);
    out.append(";REMOTE_HOST=").append(tmp);
    reader_("REMOTE_ADDR", tmp);
    out.append(";REMOTE_ADDR=").append(tmp);
    reader_("AUTH_TYPE", tmp);
    out.append(";AUTH_TYPE=").append(tmp);
    reader_("CONTENT_TYPE", tmp);
    out.append(";CONTENT_TYPE=").append(tmp);
    reader_("HTTP_USER_AGENT", tmp);
    out.append(";HTTP_USER_AGENT=").append(tmp);
    reader_("HTTP_ACCEPT", tmp);
    out.append(";HTTP_ACCEPT=").append(tmp);
    reader_("HTTP_REFERER", tmp);
    out.append(";HTTP_REFERER=").append(tmp);
    reader_("HTTP_COOKIE", tmp);
    out.append(";HTTP_COOKIE=").append(tmp);
}

cookie* enviroment::get_cookie( const std::string& name )
{
    cookies_map::iterator iter = cookies_.find(name);
    if (iter == cookies_.end())
    {
        return NULL;
    }

    return &(iter->second);
}

cookie* enviroment::get_cookie( const char* name )
{
    if (NULL == name)
    {
        return NULL;
    }

    return get_cookie(string(name));
}

std::string* ctk::http::enviroment::get_query( const std::string& k )
{
    str_vec *vec = get_queries(k);
    if (NULL == vec || vec->empty())
    {
        return NULL;
    }

    return &(vec->at(0));
}

std::string* ctk::http::enviroment::get_query( const char* k )
{
    if (NULL == k)
    {
        return NULL;
    }

    return get_query(string(k));
}

str_vec* ctk::http::enviroment::get_queries( const std::string& k )
{
    form_map_i iter = query_string_fm_.find(k);
    if (query_string_fm_.end() == iter)
    {
        return NULL;
    }

    return &(iter->second);
}

str_vec* ctk::http::enviroment::get_queries( const char* k )
{
    if (NULL == k)
    {
        return NULL;
    }

    return get_queries(string(k));
}


http_request_method enviroment::parse_method( const std::string& method )
{
    if (0 == strncasecmp(method.c_str(), "GET", method.length()))
    {
        return hrm_get;
    }

    if (0 == strncasecmp(method.c_str(), "POST", method.length()))
    {
        return hrm_post;
    }

    if (0 == strncasecmp(method.c_str(), "PUT", method.length()))
    {
        return hrm_put;
    }

    if (0 == strncasecmp(method.c_str(), "DELETE", method.length()))
    {
        return hrm_delete;
    }

    if (0 == strncasecmp(method.c_str(), "HEAD", method.length()))
    {
        return hrm_head;
    }

    if (0 == strncasecmp(method.c_str(), "TRACE", method.length()))
    {
        return hrm_trace;
    }

    if (0 == strncasecmp(method.c_str(), "OPTIONS", method.length()))
    {
        return hrm_options;
    }

    if (0 == strncasecmp(method.c_str(), "CONNECT", method.length()))
    {
        return hrm_connect;
    }

    return hrm_unknown;
}

void enviroment::parse_cookie( const std::string& cookies )
{
    http::parse_cookie(cookies, cookies_);
}

void ctk::http::enviroment::parse_query_string( const std::string& qs )
{
    http::parse_query_string(qs, query_string_fm_);
}

void ctk::http::enviroment::clear()
{
    server_port_ = 0;
    content_length_ = 0;
    is_https_ = false;
    method_ = hrm_unknown;

    server_name_.clear();
    path_info_.clear();
    path_translated_.clear();
    script_name_.clear();
    query_string_.clear();
    remote_host_.clear();
    remote_addr_.clear();
    auth_type_.clear();
    content_type_.clear();
    user_agent_.clear();
    accept_.clear();
    referer_.clear();
    
    cookies_.clear();
    query_string_fm_.clear();

}
