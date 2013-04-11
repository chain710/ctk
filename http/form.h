/**
 * file: form.h
 * author: marcuscai
 * date: 2011-8-30
 * desc: 
 */

#ifndef _CTK_FORM_H_
#define _CTK_FORM_H_

#include <string>
#include <map>
#include <vector>

namespace ctk
{
namespace http
{
    // http://www.ietf.org/rfc/rfc2045.txt
    // 6. Content-Transfer-Encoding Header Field
    enum content_transfer_encoding
    {
        cte_unknown = 0,    /**< 未知的 */
        cte_7bit = 1,   /**< 默认的 */
        cte_8bit,   /**< 2 */
        cte_binary, /**< 3 */
        cte_quoted_printable,   /**< 4 */
        cte_base64, /**< 5 */
        cte_ietf_token, /**< 6 */
        cte_x_token,    /**< 7, 自定义的编码格式 */
    };

    // http://www.w3.org/TR/html4/interact/forms.html#h-17.13.4
    // User agents must support the content types listed below. Behavior for other content types is unspecified.
    // 暂时只支持这两种
    enum form_content_type
    {
        fct_unknown = 0,
        fct_form_urlencoded = 1,    // application/x-www-form-urlencoded  
        fct_multipart_formdata =2,  // multipart/form-data  
    };

    class multipart
    {
    public:
        void clear();
        content_transfer_encoding encode;   /**< Content-Transfer-Encoding */
        std::string disposition;    // Content-Disposition
        std::string name;
        std::string file;   /**< filename */
        std::string type;   /**< content-type */
        std::string content;    /**< content */
    };

    typedef std::vector<std::string> str_vec;
    typedef std::map<std::string, str_vec> form_map;
    typedef form_map::iterator form_map_i;

    typedef std::vector<multipart> multipart_vec;
    typedef std::map<std::string, multipart_vec> multipart_map;
    typedef multipart_map::iterator multipart_map_i;

    class form
    {
    public:
        form();
        ~form();

        int parse_form(const std::string &inbuf, form_content_type ct, const std::string& type);

        void append(const std::string& k, const std::string& v);
        void append(const char* k, const char* v);

        void append(const std::string& k, const str_vec& vv);
        void append(const char* k, const str_vec& vv);

        void append(const std::string& n, const multipart& mp);
        void append(const char* n, const multipart& mp);

        void append(const std::string& n, const multipart_vec& vmp);
        void append(const char* n, const multipart_vec& vmp);

        void clear();

        // 默认只取第一个
        std::string* form_value(const std::string& k);
        std::string* form_value(const char* k);

        str_vec* form_vvalue(const std::string& k);
        str_vec* form_vvalue(const char* k);

        multipart* form_mp(const std::string& n);
        multipart* form_mp(const char* n);

        multipart_vec* form_vmp(const std::string& n);
        multipart_vec* form_vmp(const char* n);
    private:
        form_map f_data_;
        multipart_map mp_data_;
    };


}
}
#endif
