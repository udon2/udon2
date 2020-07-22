#ifndef UD_UTIL_H
#define UD_UTIL_H

#include <vector>
#include <string>

namespace Util {
    std::vector<std::string> stringSplit(std::string strToSplit, char delimeter);
    std::string stringJoin(std::vector<std::string> v, char joinStr);
};

#endif