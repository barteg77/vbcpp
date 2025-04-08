#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED

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
#include "ResourceId.h"
#include "ResourceRepo.h"
#include "SoundLoader.h"
#include "RObject.h"
#include "RObjectLoader.h"

#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#define DEVELOPMENT_RESOURCES

typedef std::list<std::unique_ptr<Resource>> resourcePtrList;


class ResourceManager
{
    static const ResourceId DEFAULT_WHITE_TEXTURE_RESOURCE_ID;

    public:
        virtual ~ResourceManager();

        static ResourceManager& getInstance();

        template <class ResourceTypeT>
        ResourceTypeT* findResource(const ResourceId& resourceId) {
            const std::list<std::unique_ptr<ResourceTypeT>>& container (getResourceContainer<ResourceTypeT>());
            for (const auto& resource : container){
                if (resource->getResourceId() == resourceId) {
                    return resource.get();
                }
            }
            return nullptr;
        }

        std::string realPath(const std::string& pseudoId); // use wisely or better don't use

        template <class ResourceTypeT>
        ResourceTypeT* loadResource(const ResourceId& resourceId) {
            LOG_DEBUG("Looking for resource " + resourceId.getDebugString() + "...");
            ResourceTypeT* foundResource (findResource<ResourceTypeT>(resourceId));
            if (foundResource) {
                LOG_DEBUG("Resource " + resourceId.getDebugString() + " found in resource manager (it had been already loaded)");
                return foundResource;
            }
            LOG_DEBUG("Resource " + resourceId.getDebugString() + " not found in resource manager.");
            for (auto& resourceRepo : _resourceRepos) {
                LOG_DEBUG("Checking for resource " + resourceId.getDebugString() + " in repo " + resourceRepo->getDebugString());
                std::unique_ptr<ResourceTypeT> loadedResource (resourceRepo->loadResource<ResourceTypeT>(resourceId));
                if (loadedResource) {
                    ResourceTypeT* loadedResourceRawPtr (loadedResource.get());
                    getResourceContainer<ResourceTypeT>().push_back(std::move(loadedResource));
                    LOG_DEBUG("Resource " + resourceId.getDebugString() + " found in repo " + resourceRepo->getDebugString());
                    return loadedResourceRawPtr;
                }
            }
            LOG_ERROR("Resource " + resourceId.getDebugString() + " not found in any repository! Program will be terminated.");
            assert(false);
            return nullptr;
        }

        void reloadTexture(RTexture2D* texture);
        void reloadTexture(const ResourceId& resourceId);
        void reloadTexture(RTextureCubeMap* texture);
        void reloadAllTextures();

        RTexture2D* loadDefaultWhiteTexture();
		RTexture2D* loadOneColorTexture(glm::vec4 color);

        void reloadShader(RShader* shader);
        void reloadAllShaders();
		RObject* loadRObject(const std::string& name);
		RRoadProfile* loadRoadProfile(const std::string& name);
		RDisplayFont* loadDisplayFont(const std::string& name);

        void addResourceRepo(std::unique_ptr<ResourceRepo> resourceRepoPtr);

    private:
        std::list<std::unique_ptr<RTexture2D>> _resourcesRTexture2D;
        std::list<std::unique_ptr<RTextureCubeMap>> _resourcesRTextureCubeMap;
        std::list<std::unique_ptr<RShader>> _resourcesRShader;
        std::list<std::unique_ptr<RStaticModel>> _resourcesRStaticModel;
        std::list<std::unique_ptr<RAnimatedModel>> _resourcesRAnimatedModel;
        std::list<std::unique_ptr<RAnimation>> _resourcesRAnimation;
        std::list<std::unique_ptr<RFont>> _resourcesRFont;
        std::list<std::unique_ptr<RSound>> _resourcesRSound;
        std::list<std::unique_ptr<RObject>> _resourcesRObject;
        std::list<std::unique_ptr<RRoadProfile>> _resourcesRRoadProfile;
        std::list<std::unique_ptr<RDisplayFont>> _resourcesRDisplayFont;
        std::list<std::unique_ptr<RMaterialsCollection>> _resourcesRMaterialsCollection;
        ResourceManager();

        std::vector<std::unique_ptr<ResourceRepo>> _resourceRepos;
        //std::string getPath(const ResourceId& resourceId, const size_t partIdx=0);// to jest tylko na chwile dopoki loady nie są przeniesione do ResourceRepo
        //std::vector<std::string> getPaths(const ResourceId& resourceId);

        template <class ResourceTypeT> std::list<std::unique_ptr<ResourceTypeT>>& getResourceContainer();
};

template <> inline std::list<std::unique_ptr<RTexture2D>>& ResourceManager::getResourceContainer<RTexture2D>() { return _resourcesRTexture2D; }
template <> inline std::list<std::unique_ptr<RTextureCubeMap>>& ResourceManager::getResourceContainer<RTextureCubeMap>() { return _resourcesRTextureCubeMap; }
template <> inline std::list<std::unique_ptr<RShader>>& ResourceManager::getResourceContainer<RShader>() { return _resourcesRShader; }
template <> inline std::list<std::unique_ptr<RStaticModel>>& ResourceManager::getResourceContainer<RStaticModel>() { return _resourcesRStaticModel; }
template <> inline std::list<std::unique_ptr<RAnimatedModel>>& ResourceManager::getResourceContainer<RAnimatedModel>() { return _resourcesRAnimatedModel; }
template <> inline std::list<std::unique_ptr<RAnimation>>& ResourceManager::getResourceContainer<RAnimation>() { return _resourcesRAnimation; }
template <> inline std::list<std::unique_ptr<RFont>>& ResourceManager::getResourceContainer<RFont>() { return _resourcesRFont; }
template <> inline std::list<std::unique_ptr<RSound>>& ResourceManager::getResourceContainer<RSound>() { return _resourcesRSound; }
template <> inline std::list<std::unique_ptr<RObject>>& ResourceManager::getResourceContainer<RObject>() { return _resourcesRObject; }
template <> inline std::list<std::unique_ptr<RRoadProfile>>& ResourceManager::getResourceContainer<RRoadProfile>() { return _resourcesRRoadProfile; }
template <> inline std::list<std::unique_ptr<RDisplayFont>>& ResourceManager::getResourceContainer<RDisplayFont>() { return _resourcesRDisplayFont; }
template <> inline std::list<std::unique_ptr<RMaterialsCollection>>& ResourceManager::getResourceContainer<RMaterialsCollection>() { return _resourcesRMaterialsCollection; }

#endif // RESOURCEMANAGER_H_INCLUDED
