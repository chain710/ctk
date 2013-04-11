/**
 * file: http_request.h
 * author: marcuscai
 * date: 2011-8-30
 * desc: 
 */

#ifndef _CTK_HTTP_REQUEST_H_
#define _CTK_HTTP_REQUEST_H_

#include <tr1/functional>
#include "form.h"
#include "enviroment.h"

namespace ctk
{
namespace http
{
    typedef std::tr1::function<int (char*, size_t)> readbody_callback;

    class request
    {
    public:
        request();
        void set_read_timeout(int microsecs);
        void set_readenv_callback(const env_reader& func);
        void set_readbody_callback(const readbody_callback& func);

        int read();

        enviroment& getenv() { return env_; }
        form& getform() { return form_; }
        const std::string& getformstr() const { return http_body_; }
    private:
        readbody_callback rb_;
        env_reader re_;

        // parsed request-header
        enviroment env_;
        // parsed message-body
        form form_;

        // read http body timeout
        int timeout_rd_body_;

        // http body
        std::string http_body_;
    };
};

};
#endif
