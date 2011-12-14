#ifndef MISC_HPP_
#define MISC_HPP_

#include <boost/cstdint.hpp>

typedef boost::uint32_t UChar;
typedef std::basic_string<UChar> UString;
typedef boost::uint32_t DocId;

static UString createUString(const std::string& str)
{
    UString result;
    for(std::string::const_iterator i = str.begin(); i != str.end(); ++i)
        result.push_back(*i);
    return result;
}

#endif
