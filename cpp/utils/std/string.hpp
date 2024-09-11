#ifndef _STRING_HPP_2757FB618CDB412CBDC43D332BBAAA7B_
#define _STRING_HPP_2757FB618CDB412CBDC43D332BBAAA7B_

/************************/
/*  unordered_map.hpp   */
/*    Version 1.0       */
/*     2022/07/16       */
/************************/

#include <sstream>
#include <vector>

namespace std_ext
{
    inline std::vector<std::string> tokenize(const std::string& s, char delim = ' ')
    {
        std::vector<std::string> result;
        std::stringstream ss(s);
        std::string item;

        while (getline(ss, item, delim)) { result.push_back(item); }

        return result;
    }
} // namespace std_ext

#endif
