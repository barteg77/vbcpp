#include "ResourceId.h"


#include <iterator>
#include <sstream>
#include <string>
#include <vector>


const std::vector<std::string>& ResourceId::getIdParts() const {
    return _idParts;
}

std::string ResourceId::getIdString(size_t index) const {
    return _idParts.at(index);
}

std::string ResourceId::getDebugString() const {
    const char* const delim = ",";
    std::string semicolon(";");
    std::ostringstream result;

    std::copy(_idParts.begin(), _idParts.end(),
              std::ostream_iterator<std::string>(result, delim));

    if (!_defines.empty()){
        result<<semicolon;
        std::copy(_defines.begin(), _defines.end(),
                  std::ostream_iterator<std::string>(result, delim));
    }

    if (!_constants.empty()){
        result<<semicolon;
        for (auto it = _constants.begin(); it != _constants.end();it++){
            if (it != _constants.begin()){
                result << ",";
            }
            result << it->first << ":" << it->second;
        }
    }

    return result.str();
}