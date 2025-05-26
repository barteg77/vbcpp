#ifndef RESOURCEID_H_INCLUDED
#define RESOURCEID_H_INCLUDED

#include "assimp/Exporter.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <assert.h>
#include <memory>

enum ResourceType
{
    RT_TEXTURE,
    RT_SHADER,
    RT_MODEL,
    RT_FONT_RESOURCE,
    RT_SOUND,
    RT_OBJECT,
    RT_ROAD_PROFILE,
    RT_DISPLAY_FONT,
    RT_MATERIALS_COLLECTION,
    RT_ANIMATION,
    RT_OTHER
};

class ResourceId final
{
public:
	typedef std::vector<std::string> IdParts;
    enum class HierarchyHandling { with, without };
    typedef std::vector<std::string> Defines;
    typedef std::unordered_map<std::string, std::string> Constants;
    typedef int FontPixelSize;

private:
    ResourceType _resourceType;
    IdParts _idParts;
    HierarchyHandling _hierarchyHandling;
    const std::unique_ptr<const ResourceId> _highPolyModelId;
    Defines _defines;
    Constants _constants;
    FontPixelSize _fontPixelSize;

    explicit ResourceId(const ResourceType& resourceType,
                        const IdParts& idParts,
                        const HierarchyHandling hierarchyHandling,
                        std::unique_ptr<const ResourceId>&& highPolyModelId,
                        const Defines& defines,
                        const Constants& constants,
                        const FontPixelSize& fontPixelSize)
    : _resourceType(resourceType),
      _idParts(idParts),
      _hierarchyHandling(hierarchyHandling),
      _highPolyModelId(std::move(highPolyModelId)),
      _defines(defines),
      _constants(constants),
      _fontPixelSize(fontPixelSize)
    {};

public:
    ResourceId(const ResourceId& old)
    : _resourceType(old._resourceType),
      _idParts(old._idParts),
      _hierarchyHandling(old._hierarchyHandling),
      _highPolyModelId(old._highPolyModelId ? std::make_unique<ResourceId>(*old._highPolyModelId) : nullptr),
      _defines(old._defines),
      _constants(old._constants),
      _fontPixelSize(old._fontPixelSize)
    {};

    template <ResourceType resourceType>
    static ResourceId create (const std::string& id)
    {
        static_assert(resourceType == RT_TEXTURE
                   || resourceType == RT_SHADER // compute shader with default defines, constants
                   || resourceType == RT_SOUND
                   || resourceType == RT_OBJECT
                   || resourceType == RT_ROAD_PROFILE
                   || resourceType == RT_DISPLAY_FONT
                   || resourceType == RT_MATERIALS_COLLECTION
                   || resourceType == RT_ANIMATION
                   || resourceType == RT_OTHER,
                      "incorrect resource type for this function");
        
        if (resourceType == RT_DISPLAY_FONT) {
            std::vector<int> displayFontSizes { 5, 7, 9, 12, 16 };
            IdParts idParts;
            for (const auto& displayFontSize : displayFontSizes) {
                idParts.push_back(id+std::to_string(displayFontSize)+".csv");
            }
            return ResourceId(resourceType,
                              idParts,
                              HierarchyHandling::without,
                              nullptr,
                              {},
                              {},
                              0
                              );
        } else {
            return ResourceId(resourceType,
                              {id},
                              HierarchyHandling::without,
                              nullptr,
                              {},
                              {},
                              0
                              );
        }
    }

    template <ResourceType resourceType>
    static ResourceId create (const std::string& id,
                              const HierarchyHandling hierarchyHandling)
    {
        static_assert(resourceType == RT_MODEL, "incorrect resource type for this function");
        
        return ResourceId(resourceType,
                          {id},
                          hierarchyHandling,
                          nullptr,
                          {},
                          {},
                          0
                          );
    }

    template <ResourceType resourceType>
    static ResourceId create (const std::string& id,
                              const HierarchyHandling hierarchyHandling,
                              const ResourceId& highPolyModelId)
    {
        static_assert(resourceType == RT_MODEL, "incorrect resource type for this function");
        
        return ResourceId(resourceType,
                          {id},
                          hierarchyHandling,
                          std::make_unique<ResourceId>(highPolyModelId),
                          {},
                          {},
                          0
                          );
    }

    template <ResourceType resourceType>
    static ResourceId create (const std::string& vertexPath,
                              const std::string& fragmentPath,
                              const Defines& defines = Defines(),
                              const Constants& constants = Constants())
    {
        static_assert(resourceType == RT_SHADER, "incorrect resource type for this function");
        return ResourceId(resourceType,
                          {vertexPath, fragmentPath},
                          HierarchyHandling::without,
                          nullptr,
                          defines,
                          constants,
                          0
                          );
    }

    // compute shader
    template <ResourceType resourceType>
    static ResourceId create (const std::string& computePath,
                              const Defines& defines, // default-defines case is handled by another function
                              const Constants& constants = Constants())
    {
        static_assert(resourceType == RT_SHADER, "incorrect resource type for this function");
        return ResourceId(resourceType,
                          {computePath},
                          HierarchyHandling::without,
                          nullptr,
                          defines,
                          constants,
                          0
                          );
    }
    
    template<ResourceType resourceType>
    static ResourceId create (const IdParts& idParts)
    {
        static_assert(resourceType == RT_TEXTURE, "incorrect resource type for this function");
        return ResourceId(resourceType,
                          idParts,
                          HierarchyHandling::without,
                          nullptr,
                          {},
                          {},
                          0
                          );
    }

    template<ResourceType resourceType>
    static ResourceId create (const std::string& id,
                              const FontPixelSize& fontPixelSize)
    {
        static_assert(resourceType == RT_FONT_RESOURCE, "incorrect resourcetype for this function");
        return ResourceId(resourceType,
                          {id},
                          HierarchyHandling::without,
                          nullptr,
                          {},
                          {},
                          fontPixelSize
                          );
    }
    ResourceType getResourceType() const
    { return _resourceType; }

    size_t getIdPartsCount() const
    { return _idParts.size(); }

    const std::vector<std::string>& getIdParts() const;

    std::string getIdString(size_t index) const;

    HierarchyHandling getHierarchy() const
    { return _hierarchyHandling; }

    const ResourceId* getHighPolyModelId() const
    { return _highPolyModelId.get(); }

    Defines getDefines() const
    { return _defines; }

    Constants getConstants() const
    { return _constants; }

    int getFontPixelSize() const
    {
        assert(_resourceType == RT_FONT_RESOURCE);
        return _fontPixelSize;
    }

    std::string getDebugString() const;

    bool operator==(const ResourceId& other)
    { return _resourceType == other._resourceType
          && _idParts == other._idParts
          && _defines == other._defines
          && _constants == other._constants
          && _fontPixelSize == other._fontPixelSize; }
    
    bool operator!=(const ResourceId& other)
    { return _resourceType != other._resourceType
          || _idParts != other._idParts
          || _defines != other._defines
          || _constants != other._constants
          || _fontPixelSize != other._fontPixelSize; }
};


#endif // RESOURCEID_H_INCLUDED
