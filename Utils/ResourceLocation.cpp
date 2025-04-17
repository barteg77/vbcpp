#include "ResourceLocation.h"

std::string ResourceLocation::getPath() const {
    assert(id.getIdPartsCount() == 1); // getPath allowed only for resources with single id part
    return repo.getPath() + "/" + id.getIdString(0);
}

std::vector<std::string> ResourceLocation::getPaths() const {
    std::vector<std::string> retval;
    for (const auto& idPart : id.getIdParts()) {
        retval.push_back(repo.getPath() + "/" + idPart);
    }
    return retval;
}