#ifndef GUARD_UtilFunctions_h
#define GUARD_UtilFunctions_h

namespace utilityFunctions
{

    template <typename T>
    static void getListFromString(std::string& str, std::vector<T>& list)
    { // Simple function that extracts the templated object from a string.
        list.clear();
        std::stringstream strm(str);
        while(true) {
            T n;   strm >> n;
            if(!strm) break;
            list.push_back(n);
        }
    }

};

#endif
