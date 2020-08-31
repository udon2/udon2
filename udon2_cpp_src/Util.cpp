/*
 * Copyright 2020 Dmytro Kalpakchi
 */

#include "Util.h"

#include <sstream>

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

std::string stringJoin(std::string v[], char joinStr) {
  std::stringstream ss;
  size_t i = 0;
  while (!v[i].empty()) {
    if (i != 0) ss << joinStr;
    ss << v[i];
    i++;
  }
  return ss.str();
}
}  // namespace Util
