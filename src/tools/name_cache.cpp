/**
 * Copyright 2019 BidFX Systems Ltd. All rights reserved.
 */

#include "name_cache.h"

namespace bidfx_public_api::tools
{

NameCache NameCache::DEFAULT_CACHE = NameCache();

NameCache& NameCache::GetDefault()
{
    return DEFAULT_CACHE;
}

std::string NameCache::CreateUniqueName(const std::string& name_prefix)
{
    return name_prefix + std::to_string(NextID(name_prefix));
}

int NameCache::NextID(const std::string& name_prefix)
{
    std::unique_lock<std::mutex> lock(mutex_);
    auto existing = cache_.find(name_prefix);
    if (existing != cache_.end())
    {
        return ++(existing->second);
    }
    else
    {
        cache_[name_prefix] = 1;
        return 1;
    }
}

int NameCache::LastID(const std::string& name_prefix)
{
    std::unique_lock<std::mutex> lock(mutex_);
    auto existing = cache_.find(name_prefix);
    if (existing == cache_.end())
    {
        return 0;
    }
    return existing->second;
}

std::set<std::string> NameCache::Names()
{
    std::unique_lock<std::mutex> lock(mutex_);
    std::set<std::string> names;
    for (const auto& kvp : cache_)
    {
        names.insert(kvp.first);
    }
    return names;
}

}