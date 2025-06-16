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
        std::unique_ptr<RTexture2D> texture (std::unique_ptr<RTexture2D>( RTexture2D::createWhiteTexture(resourceId, size, textureCompression)));
        return texture;
    } else if (std::regex_match(idString, matches, oneColorTextureRegex) && matches.size() == 5) {
        glm::vec4 color (std::stof(matches[1].str()),
                         std::stof(matches[2].str()),
                         std::stof(matches[3].str()),
                         std::stof(matches[4].str()));
        bool textureCompression = useCompression && GameConfig::getInstance().textureCompression;
        std::unique_ptr<RTexture2D> texture (std::unique_ptr<RTexture2D>(RTexture2D::createOneColorTexture(resourceId, color, textureCompression)));

        if ( texture ) {
            bool anisotropic = useAnisotropicFiltering && GameConfig::getInstance().anisotropicFiltering;
            float anisotropySamples = GameConfig::getInstance().anisotropySamples;
            if (anisotropic)
                texture->setAnisotropyFiltering(anisotropic, anisotropySamples);
        }
        return texture;
    } else {
        return std::unique_ptr<RTexture2D>(nullptr);
    }
}
