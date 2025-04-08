#ifndef RESOURCEREPO_H_INCLUDED
#define RESOURCEREPO_H_INCLUDED


#include <string>
//#include "Resource.h"

#include "RTexture.h"
#include "../Graphics/RShader.h"
#include "../Graphics/Model.h"
#include "../Graphics/RRoadProfile.h"
#include "../Graphics/RoadProfileLoader.h"
#include "../Graphics/RDisplayFont.h"
#include "../Graphics/RMaterialsCollection.h"
#include "../Graphics/RAnimation.h"
#include "RAnimatedModel.h"
#include "RStaticModel.h"
#include "RFont.h"
#include "RSound.h"
#include "RObject.h"
#include "RObjectLoader.h"

#include "Path.h"

class ResourceRepo
{
    friend class ResourceManager;// to jest tylko na chwile dopoki loady nie są przeniesione do ResourceRepo

	const std::string _name;

public:
    explicit ResourceRepo(const std::string& name)
    : _name(name)
    {}
    
    std::string getName() const { return _name; }
    std::string getDebugString() const
    { return "name:"+getName()+" "+getPropertiesString(); }
    
    virtual std::unique_ptr<RTexture2D> loadTexture(const ResourceId& resourceId, bool useCompression, bool mipmapping, bool useAnisotropicFiltering) = 0;
    virtual std::unique_ptr<RTextureCubeMap> loadTextureCubeMap(const ResourceId& resourceId) = 0;
    virtual std::unique_ptr<RShader> loadShader(const ResourceId& resourceId) = 0;
    virtual std::unique_ptr<RStaticModel> loadModelWithHierarchy(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing = true /*, OGLDriver* driver */) = 0;
    virtual std::unique_ptr<RStaticModel> loadModel(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing = true) = 0;
    virtual std::unique_ptr<RAnimatedModel> loadAnimatedModel(const ResourceId& resourceId, const std::string& texturePath, const std::unordered_map<std::string, BoneInfo*>& boneInfosFromExistingModel = {}) = 0;
    virtual std::unique_ptr<RAnimation> loadAnimation(const ResourceId& resourceId) = 0;
    virtual std::unique_ptr<RFont> loadFont(const ResourceId& resourceId) = 0;//loadFont(std::string path, int pixelSize = 32);
    virtual std::unique_ptr<RSound> loadSound(const ResourceId& resourceId) = 0;
    virtual std::unique_ptr<RObject> loadRObject(const ResourceId& resourceId, const std::string& originalName) = 0;
    virtual std::unique_ptr<RRoadProfile> loadRoadProfile(const ResourceId& resourceId) = 0;
    virtual std::unique_ptr<RDisplayFont> loadDisplayFont(const ResourceId& resourceId) = 0;
    virtual std::unique_ptr<RMaterialsCollection> loadMaterialsCollection(const ResourceId& resourceId) = 0;

    template <class ResourceT>
    std::unique_ptr<ResourceT> loadResource(const ResourceId& resourceId);
private:
    virtual std::string getPropertiesString() const = 0;
};

template <> inline std::unique_ptr<RTexture2D> ResourceRepo::loadResource(const ResourceId& resourceId) { return loadTexture(resourceId, true, true, true); }//defaults was true, handling non-default values to be done later
template <> inline std::unique_ptr<RTextureCubeMap> ResourceRepo::loadResource(const ResourceId& resourceId) { return loadTextureCubeMap(resourceId); }
template <> inline std::unique_ptr<RShader> ResourceRepo::loadResource(const ResourceId& resourceId) { return loadShader(resourceId); }
template <> inline std::unique_ptr<RStaticModel> ResourceRepo::loadResource(const ResourceId& resourceId) {
    return (resourceId.getHierarchy() == ResourceId::HierarchyHandling::with
            ? loadModelWithHierarchy(resourceId, Path(resourceId.getIdParts().at(0)).withoutBackPart().getString()+"/", true)
            : loadModel(resourceId, Path(resourceId.getIdParts().at(0)).withoutBackPart().getString()+"/", true));
}
template <> inline std::unique_ptr<RAnimatedModel> ResourceRepo::loadResource(const ResourceId& resourceId) { return loadAnimatedModel(resourceId, Path(resourceId.getIdParts().at(0)).withoutBackPart().getString()+"/"); }
template <> inline std::unique_ptr<RAnimation> ResourceRepo::loadResource(const ResourceId& resourceId) { return loadAnimation(resourceId); }
template <> inline std::unique_ptr<RFont> ResourceRepo::loadResource(const ResourceId& resourceId) { return loadFont(resourceId); }
template <> inline std::unique_ptr<RSound> ResourceRepo::loadResource(const ResourceId& resourceId) { return loadSound(resourceId); }
template <> inline std::unique_ptr<RObject> ResourceRepo::loadResource(const ResourceId& resourceId) { return loadRObject(resourceId, "default_name"); }
template <> inline std::unique_ptr<RRoadProfile> ResourceRepo::loadResource(const ResourceId& resourceId) { return loadRoadProfile(resourceId); }
template <> inline std::unique_ptr<RDisplayFont> ResourceRepo::loadResource(const ResourceId& resourceId) { return loadDisplayFont(resourceId); }
template <> inline std::unique_ptr<RMaterialsCollection> ResourceRepo::loadResource(const ResourceId& resourceId) { return loadMaterialsCollection(resourceId); }

#endif // RESOURCEREPO_H_INCLUDED