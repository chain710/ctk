#include <string>
#include <map>

#ifndef _CTK_HTTP_COOKIE_H_
#define _CTK_HTTP_COOKIE_H_

namespace ctk
{
namespace http
{
    // http://www.ietf.org/rfc/rfc2109.txt
    // version is not nessary [save traffic]
    class cookie
    {
    public:
        cookie();
        cookie(const cookie& c);
        cookie(const std::string& name, 
            const std::string& value, 
            const std::string& comment,
            const std::string& domain, 
            const std::string& path,
            unsigned int max_age,
            bool secure);
        cookie(const char* name,
            const char* value,
            const char* comment,
            const char* domain, 
            const char* path,
            unsigned int max_age, 
            bool secure);

        std::string to_string();

        void reset();

        void remove()
        {
            removed_ = true;
        }

        void set_max_age(unsigned int secs)
        {
            max_age_ = secs;
        }

        void set_secure(bool secure)
        {
            secure_ = secure;
        }

        void set_name(const std::string& name)
        {
            name_ = name;
        }

        void set_value(const std::string& value)
        {
            value_ = value;
        }

        void set_comment(const std::string& comment)
        {
            comment_ = comment;
        }

        void set_domain(const std::string& domain)
        {
            domain_ = domain;
        }

        void set_path(const std::string& path)
        {
            path_ = path;
        }

        void set_name(const char* name)
        {
            if (name) name_ = name;
        }

        void set_value(const char* value)
        {
            if (value) value_ = value;
        }

        void set_comment(const char* comment)
        {
            if (comment) comment_ = comment;
        }

        void set_domain(const char* domain)
        {
            if (domain) domain_ = domain;
        }

        void set_path(const char* path)
        {
            if (path) path_ = path;
        }

        unsigned int get_max_age() const
        {
            return max_age_;
        }

        bool is_secure() const
        {
            return secure_;
        }

        bool is_removed() const
        {
            return removed_;
        }

        const std::string& get_name() const
        {
            return name_;
        }

        const std::string& get_value() const
        {
            return value_;
        }

        const std::string& get_comment() const
        {
            return comment_;
        }

        const std::string& get_domain() const
        {
            return domain_;
        }

        const std::string& get_path() const
        {
            return path_;
        }
        
    private:
        unsigned int max_age_;
        bool secure_;
        bool removed_;

        std::string name_;
        std::string value_;
        std::string comment_;
        std::string domain_;
        std::string path_;
    };

    typedef std::map<std::string, cookie> cookies_map;
}
}

#endif
