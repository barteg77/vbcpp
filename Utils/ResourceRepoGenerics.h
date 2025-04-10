#ifndef RESOURCEREPOGENERICS_H_INCLUDED
#define RESOURCEREPOGENERICS_H_INCLUDED

#include "ResourceRepo.h"
#include <regex>

class ResourceRepoGenerics : public ResourceRepo {
    static const std::regex oneColorTextureRegex;

    virtual std::string getPropertiesString() const { return "type:generics"; }

public:
    explicit ResourceRepoGenerics()
    : ResourceRepo("VBCPP GENERICS")
    {}
    
    virtual std::unique_ptr<RTexture2D> loadTexture(const ResourceId& resourceId, bool useCompression, bool mipmapping, bool useAnisotropicFiltering);
    virtual std::unique_ptr<RTextureCubeMap> loadTextureCubeMap(const ResourceId& resourceId);
    virtual std::unique_ptr<RShader> loadShader(const ResourceId& resourceId);
    virtual std::unique_ptr<RStaticModel> loadModelWithHierarchy(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing = true /*, OGLDriver* driver */);
    virtual std::unique_ptr<RStaticModel> loadModel(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing = true);
    virtual std::unique_ptr<RAnimatedModel> loadAnimatedModel(const ResourceId& resourceId, const std::string& texturePath, const std::unordered_map<std::string, BoneInfo*>& boneInfosFromExistingModel = {});
    virtual std::unique_ptr<RAnimation> loadAnimation(const ResourceId& resourceId);
    virtual std::unique_ptr<RFont> loadFont(const ResourceId& resourceId);//loadFont(std::string path, int pixelSize = 32);
    virtual std::unique_ptr<RSound> loadSound(const ResourceId& resourceId);
    virtual std::unique_ptr<RObject> loadRObject(const ResourceId& resourceId, const std::string& originalName);
    virtual std::unique_ptr<RRoadProfile> loadRoadProfile(const ResourceId& resourceId);
    virtual std::unique_ptr<RDisplayFont> loadDisplayFont(const ResourceId& resourceId);
    virtual std::unique_ptr<RMaterialsCollection> loadMaterialsCollection(const ResourceId& resourceId);

    template <class ResourceT>
    std::unique_ptr<ResourceT> loadResource(const ResourceId& resourceId);
};

#endif // RESOURCEREPOGENERICS_H_INCLUDED