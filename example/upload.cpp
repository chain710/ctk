/**
 * file: upload.cpp
 * author: marcuscai
 * date: 2011-9-16
 * desc: upload file example
 */


#include "fast_cgi_wrapper.h"
#include "request.h"
#include "response.h"
#include "time_helper.h"
#include <string>
#include <sys/time.h>
#include <fstream>

using namespace std;
using namespace std::tr1;
using namespace ctk;
using namespace ctk::http;

class mycgi
{
public:
    void run();
private:
    int readbody(char*, size_t);
    void readenv(const string&, string&);
    int writersp(const char*, size_t);

    fast_cgi_wrapper fcgi_;
};

void mycgi::run()
{
    int ret = 0;
    request req;
    req.set_readbody_callback(std::tr1::bind(&mycgi::readbody, this, placeholders::_1, placeholders::_2));
    req.set_readenv_callback(std::tr1::bind(&mycgi::readenv, this, placeholders::_1, placeholders::_2));
    req.set_read_timeout(1000000);
    response rsp;
    rsp.set_writer_callback(std::tr1::bind(&mycgi::writersp, this, placeholders::_1, placeholders::_2));

    timeval beg,end;
    while (0 == (ret = fcgi_.accept()))
    {
        gettimeofday(&beg, NULL);
        ret = req.read();
        multipart *part = req.getform().form_mp("test");
        if (NULL != part)
        {
            fstream fout("/tmp/upload_tmp", ios_base::out|ios_base::trunc);
            fout.write(part->content.c_str(), part->content.length());
            fout.close();
        }

        rsp.prepare_output();
        rsp.set_status(200);
        rsp.set_content_type("text/html");
        rsp.append_body("hello, world! ");
        gettimeofday(&end, NULL);
        rsp.append_body_ex("fcgi run cost:%d\n", timeval_minus(beg, end));
        rsp.write_all();

        fcgi_.close();

    }
}

int mycgi::readbody( char* buf, size_t length)
{
    timeval timeout = {1,0};
    return fcgi_.read_all_stdin(buf, length, timeout);
}

void mycgi::readenv( const string& key, string& val)
{
    char tmp_val[128];
    fcgi_.read_param(key.c_str(), tmp_val, sizeof(tmp_val));
    val.assign(tmp_val);
}

int mycgi::writersp( const char* buf, size_t length)
{
    return fcgi_.write_stdout(buf, length);
}

int main(int argc, char** argv)
{
    mycgi cgi;
    cgi.run();

    return 0;
}
