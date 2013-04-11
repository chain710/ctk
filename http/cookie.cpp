#include "cookie.h"
#include <stdio.h>
using namespace ctk::http;
using namespace std;

cookie::cookie()
{
    reset();
}

cookie::cookie( const cookie& c )
{
    reset();
    set_max_age(c.max_age_);
    set_secure(c.secure_);
    set_name(c.name_);
    set_value(c.value_);
    set_comment(c.comment_);
    set_domain(c.domain_);
    set_path(c.path_);

    if (c.removed_)
    {
        remove();
    }
}

cookie::cookie( const std::string& name, const std::string& value, const std::string& comment, const std::string& domain, const std::string& path, unsigned int max_age, bool secure )
{
    reset();
    set_max_age(max_age);
    set_secure(secure);
    set_name(name);
    set_value(value);
    set_comment(comment);
    set_domain(domain);
    set_path(path);
}

cookie::cookie( const char* name, const char* value, const char* comment, const char* domain, const char* path, unsigned int max_age, bool secure )
{
    reset();
    set_max_age(max_age);
    set_secure(secure);
    set_name(name);
    set_value(value);
    set_comment(comment);
    set_domain(domain);
    set_path(path);
}

string cookie::to_string()
{
    string out;
    out.reserve(256);

    out.append("Set-Cookie:").append(name_).append("=").append(value_);
    if (!comment_.empty())
    {
        out.append("; Comment=").append(comment_);
    }

    if (!domain_.empty())
    {
        out.append("; Domain=").append(domain_);
    }

    if (removed_)
    {
        out.append("; Expires=Fri, 01-Jan-1971 01:00:00 GMT;");
    }
    else if (0 != max_age_)
    {
        char maxage[32];
        snprintf(maxage, sizeof(maxage), "%d", max_age_);
        out.append("; Max-Age=").append(maxage);
    }

    if (!path_.empty())
    {
        out.append("; Path=").append(path_);
    }

    if (secure_)
    {
        out.append("; Secure");
    }

    return out;
}

void cookie::reset()
{
    max_age_ = 0;
    secure_ = false;
    removed_ = false;

    name_.clear();
    value_.clear();
    comment_.clear();
    domain_.clear();
    path_.clear();
}
