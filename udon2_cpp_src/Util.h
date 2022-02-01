/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#ifndef UDON2_CPP_SRC_UTIL_H_
#define UDON2_CPP_SRC_UTIL_H_

#include <string>
#include <vector>
#include <map>

namespace Util {
typedef std::map<std::string, std::string> FeatMap;

std::vector<std::string> stringSplit(std::string strToSplit);
std::vector<std::string> stringSplit(std::string strToSplit, char delimeter);
std::string stringJoin(std::vector<std::string> v, std::string joinStr);
std::string stringJoin(std::string v[], size_t size, char joinStr);

FeatMap parseUniversalFormat(std::string feats);
std::string stringifyFeatMap(FeatMap feats);

std::string getRandomProp(std::string prop);
std::string getRandomString();

};  // namespace Util

#endif  // UDON2_CPP_SRC_UTIL_H_
