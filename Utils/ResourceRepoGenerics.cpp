#include "ResourceRepoGenerics.h"

#include "../Game/GameConfig.h"
#include "../Graphics/LoadTexture.h"

const std::regex ResourceRepoGenerics::oneColorTextureRegex(R"(^texture\((0\.\d+|1\.0+),(0\.\d+|1\.0+),(0\.\d+|1\.0+),(0\.\d+|1\.0+)\)$)");

std::unique_ptr<RTexture2D> ResourceRepoGenerics::loadTexture(const ResourceId& resourceId, bool useCompression, bool mipmapping, bool useAnisotropicFiltering) {
    const std::string& idString (resourceId.getIdString(0));
    std::smatch matches;
    if (idString == ".defaultTexture") {
        glm::uvec2 size(2, 2);
        bool textureCompression = GameConfig::getInstance().textureCompression;
        std::unique_ptr<RTexture2D> texture (std::make_unique<RTexture2D>( RTexture2D::createWhiteTexture(resourceId, size, textureCompression)));
        return texture;
    } else if (std::regex_match(idString, matches, oneColorTextureRegex) && matches.size() == 5) {
        glm::vec4 color (std::stof(matches[1].str()),
                         std::stof(matches[2].str()),
                         std::stof(matches[3].str()),
                         std::stof(matches[4].str()));
        bool textureCompression = useCompression && GameConfig::getInstance().textureCompression;
        std::unique_ptr<RTexture2D> texture (std::make_unique<RTexture2D>(RTexture2D::createOneColorTexture(resourceId, color, textureCompression)));

        if ( texture ) {
            bool anisotropic = useAnisotropicFiltering && GameConfig::getInstance().anisotropicFiltering;
            float anisotropySamples = GameConfig::getInstance().anisotropySamples;
            if (anisotropic)
                texture->setAnisotropyFiltering(anisotropic, anisotropySamples);
        }
        return texture;
    } else {
        return std::make_unique<RTexture2D>(nullptr);
    }
}

std::unique_ptr<RTextureCubeMap> ResourceRepoGenerics::loadTextureCubeMap(const ResourceId& resourceId) { return std::make_unique<RTextureCubeMap>(nullptr); }
std::unique_ptr<RShader> ResourceRepoGenerics::loadShader(const ResourceId& resourceId) { return std::make_unique<RShader>(nullptr); }
std::unique_ptr<RStaticModel> ResourceRepoGenerics::loadModelWithHierarchy(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing) { return std::make_unique<RStaticModel>(nullptr); }
std::unique_ptr<RStaticModel> ResourceRepoGenerics::loadModel(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing) { return std::make_unique<RStaticModel>(nullptr); }
std::unique_ptr<RAnimatedModel> ResourceRepoGenerics::loadAnimatedModel(const ResourceId& resourceId, const std::string& texturePath, const std::unordered_map<std::string, BoneInfo*>& boneInfosFromExistingModel) { return std::make_unique<RAnimatedModel>(nullptr); }
std::unique_ptr<RAnimation> ResourceRepoGenerics::loadAnimation(const ResourceId& resourceId) { return std::make_unique<RAnimation>(nullptr); }
std::unique_ptr<RFont> ResourceRepoGenerics::loadFont(const ResourceId& resourceId) { return std::make_unique<RFont>(nullptr); }
std::unique_ptr<RSound> ResourceRepoGenerics::loadSound(const ResourceId& resourceId) { return std::make_unique<RSound>(nullptr); }
std::unique_ptr<RObject> ResourceRepoGenerics::loadRObject(const ResourceId& resourceId, const std::string& originalName) { return std::make_unique<RObject>(nullptr); }
std::unique_ptr<RRoadProfile> ResourceRepoGenerics::loadRoadProfile(const ResourceId& resourceId) { return std::make_unique<RRoadProfile>(nullptr); }
std::unique_ptr<RDisplayFont> ResourceRepoGenerics::loadDisplayFont(const ResourceId& resourceId) { return std::make_unique<RDisplayFont>(nullptr); }
std::unique_ptr<RMaterialsCollection> ResourceRepoGenerics::loadMaterialsCollection(const ResourceId& resourceId) { return std::make_unique<RMaterialsCollection>(nullptr); }
