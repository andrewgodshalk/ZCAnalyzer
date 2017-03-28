#ifndef GUARD_UtilFunctions_h
#define GUARD_UtilFunctions_h

#include <string>
#include <sstream>
#include <vector>

namespace utilityFunctions
{
    template <typename T>
    void getListFromString(const std::string& str, std::vector<T>& list)
    { // Simple function that extracts the templated object from a string.
        list.clear();
        std::stringstream strm(str);
        while(true) {
            T n;   strm >> n;
            if(!strm) break;
            list.push_back(n);
        }
    }

    void getDelimitedListFromString(const std::string& str, std::vector<std::string>& list, const std::string delim = ";");

};

#endif
