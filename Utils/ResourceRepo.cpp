#include "ResourceRepo.h"
#include "ResourceManager.h"

ResourceRepo::~ResourceRepo() {}

std::unique_ptr<RTexture2D> ResourceRepo::loadTexture(const ResourceId& resourceId, bool useCompression, bool mipmapping, bool useAnisotropicFiltering) { return nullptr; }
std::unique_ptr<RTextureCubeMap> ResourceRepo::loadTextureCubeMap(const ResourceId& resourceId) { return nullptr; }
std::unique_ptr<RShader> ResourceRepo::loadShader(const ResourceId& resourceId) { return nullptr; }
std::unique_ptr<RStaticModel> ResourceRepo::loadModelWithHierarchy(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing) { return nullptr; }
std::unique_ptr<RStaticModel> ResourceRepo::loadModel(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing) { return nullptr; }
std::unique_ptr<RAnimatedModel> ResourceRepo::loadAnimatedModel(const ResourceId& resourceId, const std::string& texturePath, const std::unordered_map<std::string, BoneInfo*>& boneInfosFromExistingModel) { return nullptr; }
std::unique_ptr<RAnimation> ResourceRepo::loadAnimation(const ResourceId& resourceId) { return nullptr; }
std::unique_ptr<RFont> ResourceRepo::loadFont(const ResourceId& resourceId) { return nullptr; }//loadFont(std::string path, int pixelSize = 32);
std::unique_ptr<RSound> ResourceRepo::loadSound(const ResourceId& resourceId) { return nullptr; }
std::unique_ptr<RObject> ResourceRepo::loadRObject(const ResourceId& resourceId, const std::string& originalName) { return nullptr; }
std::unique_ptr<RRoadProfile> ResourceRepo::loadRoadProfile(const ResourceId& resourceId) { return nullptr; }
std::unique_ptr<RDisplayFont> ResourceRepo::loadDisplayFont(const ResourceId& resourceId) { return nullptr; }
std::unique_ptr<RMaterialsCollection> ResourceRepo::loadMaterialsCollection(const ResourceId& resourceId) { return nullptr; }

ResourceStoreResult ResourceRepo::storeMaterialsCollection(RMaterialsCollection* object) { return ResourceStoreResult(false, "store not implemented for this type of resource in this type of repo"); }

template <> std::unique_ptr<RAnimatedModel> ResourceRepo::loadResource(const ResourceId& resourceId)
{
    std::unordered_map<std::string, BoneInfo*> boneInfos;
    const ResourceId* highPolyModelId (resourceId.getHighPolyModelId());
    if (highPolyModelId) {
        boneInfos = ResourceManager::getInstance().loadResource<RAnimatedModel>(*highPolyModelId)->getBoneInfos();
    }
    
    return loadAnimatedModel(resourceId,
                             Path(resourceId.getIdParts().at(0)).withoutBackPart().getString()+"/",
                             boneInfos);
}