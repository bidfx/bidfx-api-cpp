/**
 * Copyright 2019 BidFX Systems Ltd. All rights reserved.
 */

#include <memory>
#include <string>
#include <map>
#include <set>
#include <atomic>
#include <mutex>

#ifndef  PUBLIC_API_CPP_TOOLS_NAME_CACHE_H_
#define  PUBLIC_API_CPP_TOOLS_NAME_CACHE_H_

namespace bidfx_public_api::tools
{

/**
 * This class provides a cache of names that can be used to generate unique names for things by appending an incrementing integer
 * count to a name prefix.
 * @author Liam Asman
 */
class NameCache
{
private:
    static NameCache DEFAULT_CACHE;
    std::map<std::string, std::atomic_int> cache_;
    std::mutex mutex_;
public:
    /**
     * Gets the default {@link NameCache}.
     */
    static NameCache& GetDefault();

    /**
     * Gets a unique name by adding an integer to the given prefix.
     * @param namePrefix the name prefix for a set of similar names.
     */
    std::string CreateUniqueName(const std::string& name_prefix);

    /**
     * Gets the next unique ID for a name prefix. Each call increments the cached ID for the prefix.
     * @param namePrefix the name prefix for a set of similar names.
     */
    int NextID(const std::string& name_prefix);

    /**
     * Gets the last unique ID for a name prefix. This method does no increment the count for the prefix.
     * @param namePrefix the name prefix for a set of similar names.
     */
    int LastID(const std::string& name_prefix);

    /**
     * Gets the set of all names held in the name cache.
     * @return a set of names.
     */
    std::set<std::string> Names();
};
}

#endif //PUBLIC_API_CPP_TOOLS_NAME_CACHE_H_
