#ifndef RESOURCELOCATION_H_INCLUDED
#define RESOURCELOCATION_H_INCLUDED

#include <cassert>
#include <string>

#include "ResourceId.h"
#include "ResourceRepo.h"

class ResourceLocation final
{
    friend class ResourceManager;// to jest tylko na chwile dopoki loady nie są przeniesione do ResourceRepo
	ResourceId id;
    ResourceRepo repo;

public:
    explicit ResourceLocation(const ResourceId& id,
                              const ResourceRepo& repo)
    : id(id), repo(repo) {}
    
    const ResourceId& getResourceId() const
    { return id; }
    
    std::string getPath() const;
    std::vector<std::string> getPaths() const;
};


#endif // RESOURCELOCATION_H_INCLUDED