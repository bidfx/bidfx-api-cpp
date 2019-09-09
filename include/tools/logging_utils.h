/**
 * Copyright 2019 BidFX Systems Ltd. All rights reserved.
 */

#ifndef PUBLIC_API_CPP_TOOLS_LOGGING_UTILS_H
#define PUBLIC_API_CPP_TOOLS_LOGGING_UTILS_H

#include <string>
#include <map>

namespace bidfx_public_api::tools
{
class LoggingUtils
{
public:
template< typename K, typename V>
    static std::string MapToString(const std::map<K, V> map)
    {
        std::stringstream ss;
        for(auto kvp = map.begin(); kvp != map.end(); kvp++)
        {
            ss << kvp->first << "=" << kvp->second;
            if (std::next(kvp) != map.end())
            {
                ss << ", ";
            }
        }
        return ss.str();
    }
};
}

#endif //BIDFX_PUBLIC_API_LOGGING_UTILS_H
