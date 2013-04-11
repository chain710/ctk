#include "string_helper.h"

using namespace ctk;
using namespace std;

#define WHITE_SPACE (" \f\n\r\t\v")


std::string ctk::extractBetween( const std::string& data, const std::string& sep1, const std::string& sep2 )
{
    string::size_type start, limit;
    start = data.find(sep1, 0);
    if (string::npos == start)
    {
        return "";
    }

    start += sep1.length();
    limit = data.find(sep2, start);
    if (string::npos == limit)
    {
        return "";
    }

    return data.substr(start, limit - start);
}

std::string& ctk::trim( std::string& src )
{
    string::size_type pos = src.find_last_not_of(WHITE_SPACE);
    if(pos != string::npos) 
    {
        src.erase(pos + 1);
        pos = src.find_first_not_of(WHITE_SPACE);
        if(pos != string::npos) src.erase(0, pos);
    }
    else
    {
        src.erase(src.begin(), src.end());
    }

    return src;
}

std::string ctk::trim_const( const std::string& src )
{
    string tmp(src);
    return trim(tmp);
}
