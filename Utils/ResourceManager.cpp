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
    addResourceRepo(std::make_unique<ResourceRepoGenerics>());
	addResourceRepo(std::make_unique<ResourceRepoNative>("base", "."));
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

void ResourceManager::reloadShader(RShader* shader)
{
    //temporarily disabled
    assert(false);
}

void ResourceManager::reloadAllShaders()
{
    //temporarily disabled
    assert(false);
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
