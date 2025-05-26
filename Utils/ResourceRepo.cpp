#include "ResourceRepo.h"
#include "ResourceManager.h"

ResourceRepo::~ResourceRepo() {}

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