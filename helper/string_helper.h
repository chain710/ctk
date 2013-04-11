/**
 * file: string_helper.h
 * author: marcuscai
 * date: 2011-9-1
 * desc: ×Ö·û´®¸¨Öúº¯Êý
 */

#include <string>

namespace ctk
{
    std::string extractBetween(const std::string& data, const std::string& sep1, const std::string& sep2);
    std::string& trim(std::string& src);
    std::string trim_const(const std::string& src);
}
