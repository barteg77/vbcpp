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
#include "ResourceLocation.h"
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

        Resource* findResource(const ResourceId& resourceId);
        ResourceRepo* findRepoOfResource(const ResourceId& resourceId);
        ResourceLocation findResourceLocation(const ResourceId& resourceId);
        std::string realPath(const std::string& pseudoId); // use wisely or better don't use

        RTexture2D* loadTexture(const ResourceId& resourceId, bool useCompression = true, bool mipmapping = true, bool useAnisotropicFiltering = true);
        // filesNames: pos_x, neg_x, pos_y, neg_y, pos_z, neg_z
        RTextureCubeMap* loadTextureCubeMap(const ResourceId& resourceId);
        void reloadTexture(RTexture2D* texture);
        void reloadTexture(const ResourceId& resourceId);
        void reloadTexture(RTextureCubeMap* texture);
        void reloadAllTextures();

        RTexture2D* loadDefaultWhiteTexture();
		RTexture2D* loadOneColorTexture(glm::vec4 color);

        RShader* loadShader(const ResourceId& resourceId);
        void reloadShader(RShader* shader);
        void reloadAllShaders();

        RStaticModel* loadModelWithHierarchy(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing = true /*, OGLDriver* driver */);
        //RStaticModel* loadModelWithHierarchyOnlyNode(std::string path, std::string texturePath, std::string nodeToLoadName, Transform& loadedNodeTransformInModel,
		//											 bool normalsSmoothing = true);
        //void loadModelWithHierarchyOnlyNodes(std::string path, std::string texturePath, std::vector<std::string> nodesToLoadNames,
        //                                     std::vector<Transform>& loadedNodesTransformsInModel, std::vector<RStaticModel*>& loadedNodes,
		//									 bool normalsSmoothing = true);
        RStaticModel* loadModel(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing = true);

        RAnimatedModel* loadAnimatedModel(const ResourceId& resourceId, const std::string& texturePath, const std::unordered_map<std::string, BoneInfo*>& boneInfosFromExistingModel = {});
        RAnimation* loadAnimation(const ResourceId& resourceId);

        RFont* loadFont(const ResourceId& resourceId);//loadFont(std::string path, int pixelSize = 32);

        RSound* loadSound(const ResourceId& resourceId);

		RObject* loadRObject(const std::string& name);
        RObject* loadRObject(const ResourceId& resourceId, const std::string& originalName);

		RRoadProfile* loadRoadProfile(const std::string& name);
        RRoadProfile* loadRoadProfile(const ResourceId& resourceId);

		RDisplayFont* loadDisplayFont(const std::string& name);
        RDisplayFont* loadDisplayFont(const ResourceId& resourceId);

        RMaterialsCollection* loadMaterialsCollection(const ResourceId& resourceId);

        void addResourceRepo(std::unique_ptr<ResourceRepo> resourceRepoPtr);


    protected:
        resourcePtrList _resources;

    private:
        ResourceManager();

        std::vector<std::unique_ptr<ResourceRepo>> _resourceRepos;
        std::string getPath(const ResourceId& resourceId, const size_t partIdx=0);// to jest tylko na chwile dopoki loady nie są przeniesione do ResourceRepo
        std::vector<std::string> getPaths(const ResourceId& resourceId);
};

#endif // RESOURCEMANAGER_H_INCLUDED
