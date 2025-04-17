#ifndef RESOURCEID_H_INCLUDED
#define RESOURCEID_H_INCLUDED

#include "assimp/Exporter.hpp"
#include <unordered_map>
#include <vector>
#include <string>

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
    typedef std::vector<std::string> Defines;
    typedef std::unordered_map<std::string, std::string> Constants;
    enum class NodesAction { skip, include };
    typedef std::vector<std::string> Nodes;
    typedef int FontPixelSize;

private:
    ResourceType _resourceType;
    IdParts _idParts;
    Defines _defines;
    Constants _constants;
    NodesAction _nodesAction;
    Nodes _nodes;
    FontPixelSize _fontPixelSize;

    explicit ResourceId(const ResourceType& resourceType,
                        const IdParts& idParts,
                        const Defines& defines,
                        const Constants& constants,
                        const NodesAction& nodesAction,
                        const Nodes& nodes,
                        const FontPixelSize& fontPixelSize)
    : _resourceType(resourceType),
      _idParts(idParts),
      _defines(defines),
      _constants(constants),
      _nodesAction(nodesAction),
      _nodes(nodes),
      _fontPixelSize(fontPixelSize)
    {};

public:
    template <ResourceType resourceType>
    static ResourceId create (const std::string& id)
    {
        static_assert(resourceType == RT_TEXTURE
                   || resourceType == RT_MODEL
                   || resourceType == RT_FONT_RESOURCE
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
                              {},
                              {},
                              NodesAction::skip,
                              {},
                              0
                              );
        } else {
            return ResourceId(resourceType,
                              {id},
                              {},
                              {},
                              NodesAction::skip,
                              {},
                              0
                              );
        }
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
                          defines,
                          constants,
                          NodesAction::skip,
                          {},
                          0
                          );
    }
    
    template<ResourceType resourceType>
    static ResourceId create (const IdParts& idParts)
    {
        static_assert(resourceType == RT_TEXTURE, "incorrect resource type for this function");
        return ResourceId(resourceType,
                          idParts,
                          {},
                          {},
                          NodesAction::skip,
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
                          {},
                          {},
                          NodesAction::skip,
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

    Defines getDefines() const
    { return _defines; }

    Constants getConstants() const
    { return _constants; }

    NodesAction getNodesAction() const
    { return _nodesAction; }

    Nodes getNodes() const
    { return _nodes; }

    int getFontPixelSize() const
    { return _fontPixelSize; }

    std::string getDebugString() const;

    bool operator==(const ResourceId& other)
    { return _resourceType == other._resourceType
          && _idParts == other._idParts
          && _defines == other._defines
          && _constants == other._constants
          && _nodesAction == other._nodesAction
          && _nodes == other._nodes
          && _fontPixelSize == other._fontPixelSize; }
    
    bool operator!=(const ResourceId& other)
    { return _resourceType != other._resourceType
          || _idParts != other._idParts
          || _defines != other._defines
          || _constants != other._constants
          || _nodesAction != other._nodesAction
          || _nodes != other._nodes
          || _fontPixelSize != other._fontPixelSize; }
};


#endif // RESOURCEID_H_INCLUDED
