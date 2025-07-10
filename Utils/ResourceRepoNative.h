#ifndef RESOURCEREPONATIVE_H_INCLUDED
#define RESOURCEREPONATIVE_H_INCLUDED

#include "ResourceRepo.h"
#include "FilesystemHelper.h"

/**
 * @brief ResourceRepo handling standard Virtual Bus format.
 */
class ResourceRepoNative : public ResourceRepo {
    const std::string _path;
    const std::unique_ptr<FilesystemHelper> _filesystemHelper;

public:
    /**
     * @param name name for logging purposes
     * @param path valid filesystem path of root of resources' directory structure
     */
    explicit ResourceRepoNative(const std::string& name,
                                const std::string& path)
    : ResourceRepo(name),
      _path(path),
      _filesystemHelper(new FilesystemHelper(path, *FilesHelper::getInstance()))
    {}

    ~ResourceRepoNative() override {}

    /**
     * @brief DON'T USE Get valid filesystem path of file.
     * 
     * This is temporary solution required by ResourceManager::realPath.
     */
    std::string actualResourceFilepath(const std::string& pseudoId/*1-file resource*/) {
        return getActualFilesystemFilepath(Path(pseudoId));
    }

    /**
     * @brief DON'T USE Get valid filesystem path of directory.
     * 
     * This is temporary solution required by ResourceManager::getRepoSubdirs.
     */
    std::string actualDirpath(const std::string& pseudoId) {
        return _filesystemHelper->getActualFilesystemDirpath(Path(pseudoId));
    }

private:
    std::unique_ptr<RTexture2D> loadTexture(const ResourceId& resourceId, bool useCompression, bool mipmapping, bool useAnisotropicFiltering) override;
    std::unique_ptr<RTextureCubeMap> loadTextureCubeMap(const ResourceId& resourceId) override;
    std::unique_ptr<RShader> loadShader(const ResourceId& resourceId) override;
    std::unique_ptr<RStaticModel> loadModelWithHierarchy(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing = true /*, OGLDriver* driver */) override;
    std::unique_ptr<RStaticModel> loadModel(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing = true) override;
    std::unique_ptr<RAnimatedModel> loadAnimatedModel(const ResourceId& resourceId, const std::string& texturePath, const std::unordered_map<std::string, BoneInfo*>& boneInfosFromExistingModel = {}) override;
    std::unique_ptr<RAnimation> loadAnimation(const ResourceId& resourceId) override;
    std::unique_ptr<RFont> loadFont(const ResourceId& resourceId) override;
    std::unique_ptr<RSound> loadSound(const ResourceId& resourceId) override;
    std::unique_ptr<RObject> loadRObject(const ResourceId& resourceId, const std::string& originalName) override;
    std::unique_ptr<RRoadProfile> loadRoadProfile(const ResourceId& resourceId) override;
    std::unique_ptr<RDisplayFont> loadDisplayFont(const ResourceId& resourceId) override;
    std::unique_ptr<RMaterialsCollection> loadMaterialsCollection(const ResourceId& resourceId) override;

    ResourceStoreResult storeMaterialsCollection(RMaterialsCollection* object) override;

    // returns real (case sensitive) file path of given case-insensitive filepath if exists
    // returns empth string if wanted file doesn't exist
    std::string getActualFilesystemFilepath(Path filePath) {
        return _filesystemHelper->getActualFilesystemFilepath(filePath);
    }

    std::vector<std::string> getAllActualFilesystemFilepaths(const ResourceId& resourceId);
    std::string getPropertiesString() const override {
        return "type:native path:"+_path;
    }
};
#endif // RESOURCEREPONATIVE_H_INCLUDED