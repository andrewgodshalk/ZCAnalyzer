
#include "UtilFunctions.h"

void utilityFunctions::getDelimitedListFromString(const std::string& str, std::vector<std::string>& list, const std::string delim)
{ // Simple function that extracts the templated objects from a string, delimiited by the given character.
  // NOTE: Only works if delimiter has white space on both sides.
    list.clear();
    std::stringstream strm(str);
    std::string piece, whole;
    while(true)
    {   strm >> piece;
        if(!strm) break;
        if(piece == delim)
        {   list.push_back(whole);
            whole = "";
        }
        else
        {   if(whole != "") whole += " ";
            whole += piece;
        }
    }
    list.push_back(whole);
}
