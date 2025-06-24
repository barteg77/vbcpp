#include "ResourceRepoNative.h"
#include "../Game/Directories.h"
#include "../Game/GameConfig.h"
#include "../Graphics/LoadTexture.h"
#include "../Graphics/StaticModelLoader.h"
#include "../Graphics/AnimatedModelLoader.h"
#include "../Graphics/AnimationLoader.h"
#include "../GUI/FontLoader.h"
#include "../Graphics/MaterialSaver.h"

std::unique_ptr<RTexture2D> ResourceRepoNative::loadTexture(const ResourceId& resourceId, bool useCompression, bool mipmapping, bool useAnisotropicFiltering) {
    const std::string filePath(_filesystemHelper->getActualFilesystemFilepath(resourceId.getIdString(0)));
    if (filePath.empty()) { return nullptr; }
    bool textureCompression = useCompression && GameConfig::getInstance().textureCompression;
    std::unique_ptr<RTexture2D> texture (::loadTexture(resourceId, filePath, textureCompression, mipmapping));

    if ( texture )
    {
		bool anisotropic = useAnisotropicFiltering && GameConfig::getInstance().anisotropicFiltering;
		float anisotropySamples = GameConfig::getInstance().anisotropySamples;
		if (anisotropic)
			texture->setAnisotropyFiltering(anisotropic, anisotropySamples);
    }
    return texture;
}

std::unique_ptr<RTextureCubeMap> ResourceRepoNative::loadTextureCubeMap(const ResourceId& resourceId) {
    const std::vector<std::string> filePaths(getAllActualFilesystemFilepaths(resourceId));
    for (const std::string& filePath : filePaths) {
        if (filePath.empty()) {
            return nullptr;
        }
    }
    std::unique_ptr<RTextureCubeMap> texture (::loadTextureCubeMap(resourceId, filePaths, true));
    return texture;
}

std::unique_ptr<RShader> ResourceRepoNative::loadShader(const ResourceId& resourceId) {
    if (resourceId.getIdPartsCount() == 2) { // normal shader // shader type storage in ResourceId to be reworked because this is not clear and error-prone
        const std::vector<std::string> filePaths (getAllActualFilesystemFilepaths(resourceId));
        const std::string& vertexShaderFileName = filePaths.at(0);
        const std::string& fragmentShaderFileName = filePaths.at(1);
        if (vertexShaderFileName.empty() || fragmentShaderFileName.empty()) {
            return nullptr;
        }
        std::unique_ptr<RShader> shader (std::make_unique<RShader>(resourceId, ShaderLoader::loadShader(vertexShaderFileName,
                                                                                                        fragmentShaderFileName,
                                                                                                        resourceId.getDefines(),
                                                                                                        resourceId.getConstants()),
                                                                                                        ST_NORMAL));
        return shader;
    } else { // compute shader
        const std::string computeShaderFilePath(_filesystemHelper->getActualFilesystemFilepath(resourceId.getIdString(0)));
        if (computeShaderFilePath.empty()) {
            return nullptr;
        }
        std::unique_ptr<RShader> shader (std::make_unique<RShader>(resourceId, ShaderLoader::loadComputeShader(computeShaderFilePath.c_str(),
                                                                                                               resourceId.getDefines(),
                                                                                                               resourceId.getConstants()),
                                                                                                               ST_COMPUTE));
        return shader;
    }
}

std::unique_ptr<RStaticModel> ResourceRepoNative::loadModelWithHierarchy(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing /*, OGLDriver* driver */) {
    const std::string filePath (_filesystemHelper->getActualFilesystemFilepath(resourceId.getIdString(0)));
    if (filePath.empty()) { return nullptr; }
    StaticModelLoader loader(normalsSmoothing);
    std::unique_ptr<RStaticModel> model (loader.loadModelWithHierarchy(resourceId, filePath, texturePath)); // loader returns pointer!
    return model;
}

std::unique_ptr<RStaticModel> ResourceRepoNative::loadModel(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing) {
    const std::string filePath (_filesystemHelper->getActualFilesystemFilepath(resourceId.getIdString(0)));
    if (filePath.empty()) { return nullptr; }
    StaticModelLoader loader(normalsSmoothing);
    std::unique_ptr<RStaticModel> model (loader.loadModel(resourceId, filePath, texturePath)); // loader returns pointer!
    return model;
}

std::unique_ptr<RAnimatedModel> ResourceRepoNative::loadAnimatedModel(const ResourceId& resourceId, const std::string& texturePath, const std::unordered_map<std::string, BoneInfo*>& boneInfosFromExistingModel) {
    const std::string filePath (_filesystemHelper->getActualFilesystemFilepath(resourceId.getIdString(0)));
    if (filePath.empty()) { return nullptr; }
    AnimatedModelLoader loader;
    std::unique_ptr<RAnimatedModel> model(loader.loadAnimatedModelWithHierarchy(resourceId, filePath, texturePath, boneInfosFromExistingModel));
    return model;
}

std::unique_ptr<RAnimation> ResourceRepoNative::loadAnimation(const ResourceId& resourceId) {
    const std::string filePath (_filesystemHelper->getActualFilesystemFilepath(resourceId.getIdString(0)));
    if (filePath.empty()) { return nullptr; }
    AnimationLoader loader;
    std::unique_ptr<RAnimation> animation (loader.loadAnimation(resourceId, filePath));
    return animation;
}

std::unique_ptr<RFont> ResourceRepoNative::loadFont(const ResourceId& resourceId) {
    const std::string filePath (_filesystemHelper->getActualFilesystemFilepath(resourceId.getIdString(0)));
    if (filePath.empty()) { return nullptr; }
    FontLoader loader;
    std::unique_ptr<RFont> font (loader.loadFont(resourceId, filePath));
    return font;
}

std::unique_ptr<RSound> ResourceRepoNative::loadSound(const ResourceId& resourceId) {
    const std::string filePath (_filesystemHelper->getActualFilesystemFilepath(resourceId.getIdString(0)));
    if (filePath.empty()) { return nullptr; }
    std::unique_ptr<RSound> sound (::loadSound(resourceId, filePath));
    return sound;
}

std::unique_ptr<RObject> ResourceRepoNative::loadRObject(const ResourceId& resourceId, const std::string& originalName) {
    const std::string filePath (_filesystemHelper->getActualFilesystemFilepath(GameDirectories::OBJECTS + resourceId.getIdString(0) + "/object.xml"));
    if (filePath.empty()) { return nullptr; }
    std::unique_ptr<RObject> object (RObjectLoader::loadObject(resourceId, filePath, originalName));
    return object;
}

std::unique_ptr<RRoadProfile> ResourceRepoNative::loadRoadProfile(const ResourceId& resourceId) {
    const std::string filePath (_filesystemHelper->getActualFilesystemFilepath(resourceId.getIdString(0) + "profile.xml"));
    if (filePath.empty()) { return nullptr; }
    std::unique_ptr<RRoadProfile> roadProfile (RoadProfileLoader::loadRoadProfile(resourceId, filePath));
    return roadProfile;
}

std::unique_ptr<RDisplayFont> ResourceRepoNative::loadDisplayFont(const ResourceId& resourceId) {
    const std::vector<std::string> filePaths(getAllActualFilesystemFilepaths(resourceId));
    for (const std::string& filePath : filePaths) {
        if (filePath.empty()) {
            return nullptr;
        }
    }
    std::unique_ptr<RDisplayFont> displayFont (std::make_unique<RDisplayFont>(resourceId, filePaths));
    return displayFont;
}

std::unique_ptr<RMaterialsCollection> ResourceRepoNative::loadMaterialsCollection(const ResourceId& resourceId) {
    const std::string filePath (_filesystemHelper->getActualFilesystemFilepath(resourceId.getIdString(0)));
    if (filePath.empty()) {
        return std::unique_ptr<RMaterialsCollection>(nullptr);
    }
    std::unique_ptr<RMaterialsCollection> materialsCollection (std::make_unique<RMaterialsCollection>(resourceId));
    const std::string id_string_dir = FilesHelper::getPathToDirectoryFromFileName(resourceId.getIdString(0));

    MaterialLoader materialLoader;
    materialLoader.openFile(filePath.c_str());
    materialLoader.loadAllMaterials(materialsCollection->getMaterials(), FilesHelper::getPathToDirectoryFromFileName(id_string_dir));
    materialLoader.closeFile();
    return materialsCollection;
}

ResourceStoreResult ResourceRepoNative::storeMaterialsCollection(RMaterialsCollection* object) {
    const std::string idString (object->getResourceId().getIdString(0));
    const std::string filePath (_filesystemHelper->getActualFilesystemFilepath(idString));
    if (filePath.empty()) {
        return ResourceStoreResult(false, "file does not exist / creation not implemented yet");
    }
    const std::string texPath (FilesHelper::getPathToDirectoryFromFileName(idString));
    MaterialSaver::saveMaterials(filePath, object->getMaterials(), texPath);
    return ResourceStoreResult(true, "");
}

std::vector<std::string> ResourceRepoNative::getAllActualFilesystemFilepaths(const ResourceId& resourceId) {
    const std::vector<std::string>& idParts(resourceId.getIdParts());
    std::vector<std::string> filesNames;
    filesNames.reserve(idParts.size());
    for (const std::string& idPart : idParts) {
        filesNames.push_back(_filesystemHelper->getActualFilesystemFilepath(idPart));
    }
    return filesNames;
}
