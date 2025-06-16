#ifndef RESOURCEREPOGENERICS_H_INCLUDED
#define RESOURCEREPOGENERICS_H_INCLUDED

#include "ResourceRepo.h"
#include <regex>

/**
 * Provides generics like one color textures etc.
 */
class ResourceRepoGenerics : public ResourceRepo {
    static const std::regex oneColorTextureRegex;

    virtual std::string getPropertiesString() const { return "type:generics"; }

public:
    /**
     * ResourceRepoGenerics constructor
     */
    explicit ResourceRepoGenerics()
    : ResourceRepo("VBCPP GENERICS")
    {}

    virtual ~ResourceRepoGenerics() {}

private:
    virtual std::unique_ptr<RTexture2D> loadTexture(const ResourceId& resourceId, bool useCompression, bool mipmapping, bool useAnisotropicFiltering);
};

#endif // RESOURCEREPOGENERICS_H_INCLUDED