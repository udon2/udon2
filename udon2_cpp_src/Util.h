/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_UTIL_H_
#define UDON2_CPP_SRC_UTIL_H_

#include <string>
#include <vector>

namespace Util {
std::vector<std::string> stringSplit(std::string strToSplit, char delimeter);
std::string stringJoin(std::vector<std::string> v, char joinStr);
std::string stringJoin(std::string v[], char joinStr);
};  // namespace Util

#endif  // UDON2_CPP_SRC_UTIL_H_
