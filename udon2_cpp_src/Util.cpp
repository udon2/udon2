/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include "Util.h"

#include <sstream>
#include <iostream>

namespace Util {
std::vector<std::string> stringSplit(std::string strToSplit, char delimeter) {
  std::stringstream ss(strToSplit);
  std::string item;
  std::vector<std::string> splittedStrings;
  while (getline(ss, item, delimeter)) {
    if (!item.empty()) splittedStrings.push_back(item);
  }
  return splittedStrings;
}

std::string stringJoin(std::vector<std::string> v, char joinStr) {
  std::stringstream ss;
  for (std::vector<std::string>::size_type i = 0; i < v.size(); ++i) {
    if (i != 0) ss << joinStr;
    ss << v[i];
  }
  return ss.str();
}

std::string stringJoin(std::string v[], int size, char joinStr) {
  std::stringstream ss;
  for (size_t i = 0; i < size; i++) {
    if (i != 0) ss << joinStr;
    ss << v[i];
  }
  return ss.str();
}

FeatMap parseUniversalFormat(std::string feats) {
  FeatMap ufeats = FeatMap();
  std::vector<std::string> featsVec = stringSplit(feats, '|');
  for (std::string f : featsVec) {
    std::vector<std::string> kv = stringSplit(f, '=');
    if (kv.size() == 2) {
      ufeats[kv[0]] = kv[1];
    }
  }
  return ufeats;
}

std::string stringifyFeatMap(FeatMap feats) {
  // Create a map iterator and point to beginning of map
  std::map<std::string, std::string>::iterator it = feats.begin();

  std::vector<std::string> vec;

  // Iterate over the map using Iterator till end.
  while (it != feats.end()) {
    vec.push_back(it->first + "=" + it->second);
    it++;
  }
  return stringJoin(vec, '|');
}
}  // namespace Util
