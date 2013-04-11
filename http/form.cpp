#include "form.h"
#include "str_aux.h"

using namespace ctk::http;
using namespace std;

form::form()
{

}

form::~form()
{

}

void form::append( const std::string& k, const std::string& v )
{
    f_data_[k].push_back(v);
}

void form::append( const char* k, const char* v )
{
    if (NULL == k || NULL == v)
    {
        return;
    }

    append(string(k), string(v));
}

void form::append( const std::string& k, const str_vec& vv )
{
    str_vec &vec = f_data_[k];
    vec.insert(vec.end(), vv.begin(), vv.end());
}

void form::append( const char* k, const str_vec& vv )
{
    if (NULL == k)
    {
        return;
    }

    append(string(k), vv);
}

void form::append( const char* n, const multipart& mp )
{
    append(string(n), mp);
}


void form::append( const std::string& n, const multipart& mp )
{
    mp_data_[n].push_back(mp);
}

void form::append( const char* n, const multipart_vec& vmp )
{
    if (NULL == n)
    {
        return;
    }

    append(string(n), vmp);
}


void form::append( const std::string& n, const multipart_vec& vmp )
{
    multipart_vec &vec = mp_data_[n];
    vec.insert(vec.end(), vmp.begin(), vmp.end());
}

void form::clear()
{
    f_data_.clear();
    mp_data_.clear();
}

string* form::form_value( const std::string& k )
{
    str_vec* vec = form_vvalue(k);
    if (NULL == vec || vec->empty())
    {
        return NULL;
    }

    return &(vec->at(0));
}

string* form::form_value( const char* k )
{
    if (NULL == k)
    {
        return NULL;
    }

    return form_value(string(k));
}

str_vec* form::form_vvalue( const std::string& k )
{
    form_map::iterator iter = f_data_.find(k);
    if (iter == f_data_.end())
    {
        return NULL;
    }

    return &(iter->second);
}

str_vec* form::form_vvalue( const char* k )
{
    if (NULL == k)
    {
        return NULL;
    }

    return form_vvalue(string(k));
}

multipart* form::form_mp( const std::string& n )
{
    multipart_vec* vec = form_vmp(n);
    if (NULL == vec || vec->empty())
    {
        return NULL;
    }

    return &(vec->at(0));
}

multipart* form::form_mp( const char* n )
{
    if (NULL == n)
    {
        return NULL;
    }

    return form_mp(string(n));
}

multipart_vec* form::form_vmp( const std::string& n )
{
    multipart_map_i iter = mp_data_.find(n);
    if (iter == mp_data_.end())
    {
        return NULL;
    }

    return &(iter->second);
}

multipart_vec* form::form_vmp( const char* n )
{
    if (NULL == n)
    {
        return NULL;
    }

    return form_vmp(string(n));
}

int form::parse_form( const std::string &inbuf, form_content_type ct, const std::string& type )
{
    // multipart or urlencode?
    switch (ct)
    {
    case fct_multipart_formdata:
        parse_multi_part(inbuf, type, mp_data_);
        break;
    case fct_form_urlencoded:
    default:
        parse_query_string(inbuf, f_data_);
        break;
    }

    return 0;
}

void ctk::http::multipart::clear()
{
    encode = cte_7bit;
    disposition.clear();
    name.clear();
    file.clear();
    type.clear();
    content.clear();
}
