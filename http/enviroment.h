#include <string>
#include <map>
#include <tr1/functional>
#include "form.h"
#include "cookie.h"

namespace ctk
{
namespace http
{
    typedef std::tr1::function<void (const std::string&, std::string&)> env_reader;

    enum http_request_method
    {
        hrm_unknown = 0,
        hrm_get = 1,
        hrm_post = 2,
        hrm_put = 3,
        hrm_delete = 4,
        hrm_options = 5,
        hrm_head = 6,
        hrm_trace = 7,
        hrm_connect = 8,
    };

    class enviroment
    {
    public:
        enviroment();
        enviroment(const enviroment& env);
        explicit enviroment(const env_reader& reader);

        void set_env_reader(const env_reader& reader) { reader_ = reader; }
        int read_all();
        void clear();
        void dump_all(std::string& out);

        unsigned short get_server_port() const { return server_port_; }
        size_t get_content_length() const { return content_length_; }
        bool is_https() const { return is_https_; }
        http_request_method get_method() const { return method_; }
        const std::string& get_server_name() const { return server_name_; }
        const std::string& get_path_info() const { return path_info_; }
        const std::string& get_path_translated() const { return path_translated_; }
        const std::string& get_script_name() const { return script_name_; }
        const std::string& get_query_string() const { return query_string_; }
        const std::string& get_remote_host() const { return remote_host_; }
        const std::string& get_remote_addr() const { return remote_addr_; }
        const std::string& get_auth_type() const { return auth_type_; }
        const std::string& get_content_type() const { return content_type_; }
        const std::string& get_user_agent() const { return user_agent_; }
        const std::string& get_accept() const { return accept_; }
        const std::string& get_referer() const { return referer_; }

        // cookie op
        cookie* get_cookie(const std::string& name);
        cookie* get_cookie(const char* name);

        // querystring op
        std::string* get_query(const std::string& k);
        std::string* get_query(const char* k);

        str_vec* get_queries(const std::string& k);
        str_vec* get_queries(const char* k);
    private:
        unsigned short server_port_;
        size_t  content_length_;
        bool    is_https_;
        http_request_method method_;

        std::string server_name_;
        std::string path_info_;
        std::string path_translated_;
        std::string script_name_;
        std::string query_string_;
        std::string remote_host_;
        std::string remote_addr_;
        std::string auth_type_;
        std::string content_type_;
        std::string user_agent_;
        std::string accept_;
        std::string referer_;

        cookies_map cookies_;
        form_map query_string_fm_;

        env_reader reader_;

        // private method
        void parse_cookie(const std::string& cookies);
        void parse_query_string(const std::string& qs);
        http_request_method parse_method(const std::string& method);
    };
}
}
