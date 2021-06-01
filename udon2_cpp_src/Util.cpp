/*
 * Copyright 2021 Dmytro Kalpakchi
 */

#include "Util.h"

#include <stdlib.h>

#include <sstream>
#include <iostream>
#include <functional>
#include <random>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

#include "constants.h"

namespace Util {
std::vector<std::string> stringSplit(std::string strToSplit) {
  std::vector<std::string> splittedStrings;
  boost::algorithm::split(splittedStrings, strToSplit,
                          boost::algorithm::is_space());
  return splittedStrings;
}

std::vector<std::string> stringSplit(std::string strToSplit, char delimeter) {
  std::vector<std::string> splittedStrings;
  boost::algorithm::split(splittedStrings, strToSplit,
                          std::bind1st(std::equal_to<char>(), delimeter));
  return splittedStrings;
}

std::string stringJoin(std::vector<std::string> v, std::string joinStr) {
  return boost::algorithm::join(v, joinStr);
}

std::string stringJoin(std::string v[], size_t size, char joinStr) {
  std::stringstream ss;
  for (size_t i = 0; i < size; i++) {
    if (i != 0) ss << joinStr;
    ss << v[i];
  }
  return ss.str();
}

FeatMap parseUniversalFormat(std::string feats) {
  FeatMap ufeats = FeatMap();
  std::vector<std::string> featsVec;
  boost::algorithm::split(featsVec, feats,
                          std::bind1st(std::equal_to<char>(), '|'));
  for (std::string f : featsVec) {
    std::vector<std::string> kv;
    boost::algorithm::split(kv, f, std::bind1st(std::equal_to<char>(), '='));
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
  return stringJoin(vec, "|");
}

std::string getRandomString() {
  std::string str("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

  std::random_device rd;
  std::mt19937 generator(rd());

  std::shuffle(str.begin(), str.end(), generator);

  return str.substr(0, std::rand() % 20 + 3);
}

std::string getRandomProp(std::string prop) {
  std::string value = "";
  if (prop == "upos") {
    int rnd = std::rand() % constants::NUM_UPOS_TAGS;
    value = constants::UPOS_TAGS[rnd].code;
  } else if (prop == "deprel") {
    int rnd = std::rand() % constants::NUM_DEPRELS;
    value = constants::DEPRELS[rnd].code;
  } else if (prop == "form" || prop == "lemma") {
    value = getRandomString();
  }
  return value;
}
}  // namespace Util
