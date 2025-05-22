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

//#define DEVELOPMENT_RESOURCES

typedef std::list<std::unique_ptr<Resource>> resourcePtrList;

/**
 * @brief General interface to game resources.
 * 
 * Loads Resources from ResourceRepos and stores them in memory.
 */
class ResourceManager
{
    static const ResourceId DEFAULT_WHITE_TEXTURE_RESOURCE_ID;

    /**
     * @brief Get a resource which is already stored in resource manager's memory.
     * @tparam ResourceTypeT type of requested resource
     * @return raw pointer to resource or nullptr if not found
     */
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

    /**
     * @brief Load a resource from repository.
     * 
     * Tries loading a resource from repositories (in same order they were added)
     * until find a resource.
     * 
     * @tparam ResourceTypeT type of requested resource
     * @return unique_ptr to resource or to null if not found
     */
    template <class ResourceTypeT>
    std::unique_ptr<ResourceTypeT> loadResourceFromRepo(const ResourceId& resourceId) {
        for (auto& resourceRepo : _resourceRepos) {
            LOG_DEBUG("Checking for resource " + resourceId.getDebugString() + " in repo " + resourceRepo->getDebugString());
            std::unique_ptr<ResourceTypeT> loadedResource (resourceRepo->loadResource<ResourceTypeT>(resourceId));
            if (loadedResource) {
                LOG_DEBUG("Resource " + resourceId.getDebugString() + " found in repo " + resourceRepo->getDebugString());
                return loadedResource;
            }
        }
        return std::unique_ptr<ResourceTypeT>(nullptr);
    }
    public:
        virtual ~ResourceManager();

        /**
         * @brief Obtain a reference to ResourceManager instance
         * @return reference to ResourceManager instance
         */
        static ResourceManager& getInstance();

        /**
         * @brief Get valid filesystem path of file
         * 
         * This is temporary solution to allow finding a location of files which
         * store game data in native repo, but are not hadled as a resource e.g.
         * repaint config files.
         * 
         * @param pseudoId case-insensitive path to resource relative to repo root
         * @return valid file path or empty string if not found
         */
        std::string realPath(const std::string& pseudoId); // use wisely or better don't use
        
        /**
         * @brief Get subdirectories of directory from all native repos.
         * 
         * This is temporary solution to allow discovery of repaints and maps.
         * 
         * As available repaints used to be discovered by listing exact directory
         * contents and the listing of this dir with alternativeResourcesPath,
         * replacement of alternativeResourcesPath with resource repos requires
         * to save this functionality. To be replaced with decent solution later.
         * 
         * @param pseudoId path to directory relative to repo root
         * @return vector of names (std::string) of subdirectories
         */
        std::vector<std::string> getRepoSubdirs(const std::string& pseudoId);

        /**
         * @brief Load resource if not loaded and returns pointer to it.
         * @tparam ResourceTypeT type of requested resource
         * @param resourceId ResourceId of requested resource
         * @return raw pointer to resource or nullptr if not found
         */
        template <class ResourceTypeT>
        ResourceTypeT* loadResource(const ResourceId& resourceId) {
            LOG_DEBUG("Looking for resource " + resourceId.getDebugString() + "...");
            ResourceTypeT* foundResource (findResource<ResourceTypeT>(resourceId));
            if (foundResource) {
                LOG_DEBUG("Resource " + resourceId.getDebugString() + " found in resource manager (it had been already loaded)");
                return foundResource;
            }
            LOG_DEBUG("Resource " + resourceId.getDebugString() + " not found in resource manager.");

            std::unique_ptr<ResourceTypeT> loadedResource (loadResourceFromRepo<ResourceTypeT>(resourceId));
            if (loadedResource) {
                ResourceTypeT* loadedResourceRawPtr (loadedResource.get());
                getResourceContainer<ResourceTypeT>().push_back(std::move(loadedResource));
                return loadedResourceRawPtr;
            }
            LOG_ERROR("Resource " + resourceId.getDebugString() + " not found in any repository!");
            return nullptr;
        }

        /**
         * @brief Reload resource
         * 
         * Load resource with resourceId same as in one to be reloaded,
         * then inject new data to oldResource.
         * 
         * @tparam ResourceTypeT type of resource to reload, only RTexture2D, RTextureCubeMap, RShader are supported
         * @param oldResource pointer to resource to be reloaded
         */
        template <class ResourceTypeT>
        void reloadResourceOld(ResourceTypeT* oldResource) {
            const ResourceId resourceId (oldResource->getResourceId());
            LOG_DEBUG("(old) Reloading of resource " + resourceId.getDebugString());
            std::unique_ptr<ResourceTypeT> newResource (loadResourceFromRepo<ResourceTypeT>(resourceId));
            
            // this may impair newResource
            oldResource->setNewData(newResource.get());
        }

        /**
         * @brief Reload all resources of a specific type
         * @tparam ResourceTypeT type of resources to be reloaded
         */
        template <class ResourceTypeT>
        void reloadResources() {
            std::list<std::unique_ptr<ResourceTypeT>>& resourcesContainer (getResourceContainer<ResourceTypeT>());
            for (std::unique_ptr<ResourceTypeT>& resourceToReload : resourcesContainer) {
                reloadResourceOld(resourceToReload.get());
            }
        }

        /// @brief Reload all RTexture2D and RTextureCubeMap resources
        void reloadAllTextures();

        RTexture2D* loadDefaultWhiteTexture();
		RTexture2D* loadOneColorTexture(glm::vec4 color);

        void reloadAllShaders();
		RObject* loadRObject(const std::string& name);
		RRoadProfile* loadRoadProfile(const std::string& name);
		RDisplayFont* loadDisplayFont(const std::string& name);

        /**
         * @brief Add one more resource repo with lowest priority
         * @param resourceRepoPtr unique_ptr to repo, will be moved to ResourceManager
         */
        void addResourceRepo(std::unique_ptr<ResourceRepo> resourceRepoPtr);

        /**
         * @brief Store resource in repository
         * @tparam ResourceTypeT type of resource to be stored
         * @param resource Resource to be stored
         * @return status if store succeeded
         */
        template <class ResourceTypeT>
        ResourceStoreResult storeResource(ResourceTypeT* resource) {
            LOG_INFO("Storing resource " + resource->getResourceId().getDebugString() + " in top-priority repo: " + _resourceRepos.front()->getDebugString());
            ResourceStoreResult result (_resourceRepos.front()->storeResource<ResourceTypeT>(resource));
            if (!result._success) {
                LOG_ERROR("Failed to store resource: " + result._comment);
            }
            return result;
        }

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
