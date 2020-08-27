#include <sstream>
#include "Util.h"

using namespace std;

namespace Util {
    vector<string> stringSplit(string strToSplit, char delimeter) {
        stringstream ss(strToSplit);
        string item;
        vector<string> splittedStrings;
        while (getline(ss, item, delimeter)) {
            if (!item.empty())
                splittedStrings.push_back(item);
        }
        return splittedStrings;
    }

    string stringJoin(vector<string> v, char joinStr) {
        stringstream ss;
        for(vector<string>::size_type i = 0; i < v.size(); ++i) {
            if(i != 0)
                ss << joinStr;
            ss << v[i];
        }
        return ss.str();
    }

    string stringJoin(string v[], char joinStr) {
        stringstream ss;
        size_t i = 0;
        while (!v[i].empty()) {
            if(i != 0)
                ss << joinStr;
            ss << v[i];
            i++;
        }
        return ss.str();
    }
}