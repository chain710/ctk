#include "request.h"
#include "form.h"
#include "time_helper.h"
#include <string>
#include <strings.h>

using namespace std;
using namespace ctk;
using namespace ctk::http;

const int DEFAULT_READ_BODY_TIMEOUT = 3000000;
const static string CT_APP_URLENCODED("application/x-www-form-urlencoded");
const static string CT_APP_MULTIPART("multipart/form-data");

int ctk::http::request::read()
{
    if (timeout_rd_body_ <= 0)
    {
        timeout_rd_body_ = DEFAULT_READ_BODY_TIMEOUT;
    }

    form_.clear();
    env_.clear();
    http_body_.clear();

    env_.set_env_reader(re_);
    env_.read_all();

    if (env_.get_content_length() <= 0)
    {
        // we're done here
        return 0;
    }

    int rd = 0, rd_cnt = 0;
    char buf[1024];
    string body;
    body.reserve(env_.get_content_length());

    timeval tv_beg, tv_end;
    gettimeofday(&tv_beg, NULL);

    while (body.length() < env_.get_content_length())
    {
        ++rd_cnt;
        rd = rb_(buf, sizeof(buf));
        if (rd > 0)
        {
            body.append(buf, rd);
        }

        // prevent timeout
        // TODO: better solution
        if (rd_cnt > 50)
        {
            gettimeofday(&tv_end, NULL);
            rd_cnt = 0;

            if (timeval_minus(tv_beg, tv_end) >= timeout_rd_body_)
            {
                return -1;
            }
        }
    }

    form_content_type fct = fct_unknown;
    if ( 0 == strncasecmp(env_.get_content_type().c_str(), CT_APP_URLENCODED.c_str(), CT_APP_URLENCODED.length()) )
    {
        fct = fct_form_urlencoded;
    }
    else if ( 0 == strncasecmp(env_.get_content_type().c_str(), CT_APP_MULTIPART.c_str(), CT_APP_MULTIPART.length()) )
    {
        fct = fct_multipart_formdata;
    }
    
    http_body_ = body;
    return form_.parse_form(body, fct, env_.get_content_type());
}

void ctk::http::request::set_read_timeout( int microsecs )
{
    timeout_rd_body_ = microsecs;
}

void ctk::http::request::set_readenv_callback( const env_reader& func )
{
    re_ = func;
}

void ctk::http::request::set_readbody_callback( const readbody_callback& func )
{
    rb_ = func;
}

ctk::http::request::request()
{
    timeout_rd_body_ = DEFAULT_READ_BODY_TIMEOUT;
}
