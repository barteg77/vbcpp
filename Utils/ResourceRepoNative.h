#ifndef RESOURCEREPONATIVE_H_INCLUDED
#define RESOURCEREPONATIVE_H_INCLUDED

#include "ResourceRepo.h"
#include "ResourceRepoFilesystemHelper.h"

class ResourceRepoNative : public ResourceRepo {
    const std::string _path;
    const std::unique_ptr<FilesystemHelper> _filesystemHelper;

public:
    explicit ResourceRepoNative(const std::string& name,
                                const std::string& path)
    : ResourceRepo(name),
      _path(path),
      _filesystemHelper(new FilesystemHelper(path, *FilesHelper::getInstance()))
    {}

    virtual ~ResourceRepoNative() {}

    // temporatry solution for loaders outside resource manager
    std::string actualResourceFilepath(const std::string& pseudoId/*1-file resource*/) {
        return getActualFilesystemFilepath(Path(pseudoId));
    }

    std::string actualDirpath(const std::string& pseudoId) {
        return _filesystemHelper->getActualFilesystemDirpath(Path(pseudoId));
    }

private:
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

    virtual ResourceStoreResult storeMaterialsCollection(RMaterialsCollection* object);

    // returns real (case sensitive) file path of given case-insensitive filepath if exists
    // returns empth string if wanted file doesn't exist
    std::string getActualFilesystemFilepath(Path filePath) {
        return _filesystemHelper->getActualFilesystemFilepath(filePath);
    }

    std::vector<std::string> getAllActualFilesystemFilepaths(const ResourceId& resourceId);
    virtual std::string getPropertiesString() const{
        return "type:native path:"+_path;
    }
};
#endif // RESOURCEREPONATIVE_H_INCLUDED