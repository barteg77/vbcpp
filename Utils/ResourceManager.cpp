#include "ResourceManager.h"

#include "FilesHelper.h"
#include "../Graphics/LoadTexture.h"
#include "../Graphics/LoadShader.h"
#include "../Graphics/StaticModelLoader.h"
#include "../Graphics/AnimatedModelLoader.h"
#include "../Graphics/AnimationLoader.h"
#include "../GUI/FontLoader.h"
#include "../Game/Directories.h"
#include "../Game/GameConfig.h"
#include "Logger.h"
#include "ResourceId.h"
#include "ResourceRepoGenerics.h"
#include "ResourceRepoNative.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <iomanip>

static std::unique_ptr<ResourceManager> rsInstance;

const ResourceId ResourceManager::DEFAULT_WHITE_TEXTURE_RESOURCE_ID = ResourceId::create<RT_TEXTURE>(std::string(".defaultTexture"));

ResourceManager::ResourceManager()
{
    LOG_INFO("ResourceManager: Konstruktor");
    for (const GameConfig::RepoDefinition& repoDef : GameConfig::getInstance().resourceRepos) {
        if (repoDef._type == GameConfig::RepoType::Generics) {
            addResourceRepo(std::make_unique<ResourceRepoGenerics>());
        } else if (repoDef._type == GameConfig::RepoType::Native) {
            addResourceRepo(std::make_unique<ResourceRepoNative>(repoDef._name, repoDef._path));
        } else {
            assert(false); // unhandled repo type
        }
    }
}


ResourceManager::~ResourceManager()
{
    LOG_INFO("ResourceManager: Destruktor");

}


ResourceManager& ResourceManager::getInstance()
{
    if( !rsInstance )
        rsInstance = std::unique_ptr<ResourceManager>(new ResourceManager);

    return* rsInstance;
}

std::string ResourceManager::realPath(const std::string& pseudoId)
{
    LOG_DEBUG("Getting realPath by ResourceManager (not loading the resource, pseudo id is \"" + pseudoId + "\"");
    for (auto& resourceRepo : _resourceRepos) {
        ResourceRepoNative* resourceRepoNative (dynamic_cast<ResourceRepoNative*>(resourceRepo.get()));
        if (!resourceRepoNative) {
            continue;
        }
        std::string foundPath (resourceRepoNative->actualResourceFilepath(pseudoId));
        if (!foundPath.empty()) {
            return foundPath;
        }
    }
    assert(false);
}

std::vector<std::string> ResourceManager::getRepoSubdirs(const std::string& pseudoId)
{
    LOG_DEBUG("Listing all-repos dir \"" + pseudoId + "\" subdirs...");
    std::set<std::string> allSubdirs {};
    for (auto& resourceRepo : _resourceRepos) {
        ResourceRepoNative* resourceRepoNative (dynamic_cast<ResourceRepoNative*>(resourceRepo.get()));
        if (resourceRepoNative) {
            std::string foundPath (resourceRepoNative->actualDirpath(pseudoId));
            if (!foundPath.empty()) {
                const std::vector<std::string> foundSubdirs (FilesHelper::getInstance()->getDirectoriesList(foundPath));
                allSubdirs.insert(foundSubdirs.begin(), foundSubdirs.end());
            }
        }
    }
    const std::vector<std::string> allSubdirsV (allSubdirs.begin(), allSubdirs.end());
    return allSubdirsV;
}

void ResourceManager::reloadTexture(RTexture2D* texture)
{
    loadResource<RTexture2D>(texture->getResourceId());
}


void ResourceManager::reloadTexture(const ResourceId& resourceId)
{
    RTexture2D* texture = findResource<RTexture2D>(resourceId);
    if (texture != NULL)
    {
        reloadTexture(texture);
    }
}


void ResourceManager::reloadTexture(RTextureCubeMap* texture)
{
    //temporarily disabled
    assert(false);
    /*std::string fileNames[6];
    istringstream stream(texture->getPath());
    std::string s;
    int index = 0;
    while (getline(stream, s, ';')) {
        fileNames[index++] = s;
    }

    ::loadTextureCubeMap(fileNames, texture->getPath().c_str(), true, texture);*/
}


void ResourceManager::reloadAllTextures()
{
    assert(false);/*
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getType() == RT_TEXTURE )
        {
            std::unique_ptr<Resource>& res = *it;

            RTexture* texture = dynamic_cast<RTexture*>(res.get());
            if (texture->getTextureType() == TT_2D)
                reloadTexture(dynamic_cast<RTexture2D*>(texture));
            else if (texture->getTextureType() == TT_CUBE)
                reloadTexture(dynamic_cast<RTextureCubeMap*>(texture));
    }*/
}

RTexture2D* ResourceManager::loadDefaultWhiteTexture() {
    return ResourceManager::getInstance().loadOneColorTexture(glm::vec4(1.0, 1.0, 1.0, 1.0));
}

RTexture2D* ResourceManager::loadOneColorTexture(glm::vec4 color) {
    std::stringstream idStringS;
    idStringS << std::setprecision(3) << std::fixed;
    idStringS << "texture(" << color.r << "," << color.g << "," << color.b << "," << color.a << ")";
    // Resource repo of type ResourceRepoGenerics will handle this
    return ResourceManager::getInstance().loadResource<RTexture2D>(ResourceId::create<RT_TEXTURE>(idStringS.str()));
}

void ResourceManager::reloadAllShaders()
{
    reloadResources<RShader>();
}


RObject* ResourceManager::loadRObject(const std::string& name)
{
  return loadResource<RObject>(ResourceId::create<RT_OBJECT>(GameDirectories::OBJECTS + name + "/"));
}

RRoadProfile* ResourceManager::loadRoadProfile(const std::string& name)
{
  return loadResource<RRoadProfile>(ResourceId::create<RT_ROAD_PROFILE>(GameDirectories::ROAD_PROFILES + name + "/"));
}

RDisplayFont* ResourceManager::loadDisplayFont(const std::string& name)
{
  return loadResource<RDisplayFont>(ResourceId::create<RT_DISPLAY_FONT>(GameDirectories::DISPLAYS + name + "/"));
}


void ResourceManager::addResourceRepo(std::unique_ptr<ResourceRepo> resourceRepoPtr)
{
    _resourceRepos.push_back(std::move(resourceRepoPtr));
}
