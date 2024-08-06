#include "Path.h"
#include <cassert>

std::vector<std::string> _pathParts;

Path::Path(const std::vector<std::string>& pathParts)//use wisely, no path normalization etc.
: _pathParts(pathParts) {
    assert(std::all_of(pathParts.begin(), pathParts.end(),
            [](const std::string& pathPart)
            { return (pathPart != "." &&pathPart != ".." && std::all_of(pathPart.begin(), pathPart.end(),
                                                                        [](const char& partChar)
                                                                        { return (partChar != '/' && partChar != '\\' && partChar == std::tolower(partChar)); })); }));
}

Path::Path(const std::string& path) {
    static const std::string emptyString ("");
    static const std::string dot (".");
    static const std::string dot2 ("..");

    std::vector<std::string> tempParts;
    tempParts.push_back(emptyString);
    for (const char& pathChar : path) {
        if (pathChar == '/' || pathChar == '\\') {
            if (tempParts.back() != emptyString) {
                tempParts.push_back(emptyString);
            }
        } else {
            tempParts.back().push_back(std::tolower(pathChar));
        }
    }
    if (tempParts.back() == emptyString) {
        tempParts.pop_back();
    }

    for (const std::string& tempPart : tempParts) {
        if (tempPart == dot2) {
            if (!_pathParts.empty()) {
                _pathParts.pop_back();
            }
        } else if (tempPart != dot) {
            _pathParts.push_back(tempPart);
        }
    }
}

std::string Path::getString() const {
    std::string pathString;
    for(const std::string& pathPart : _pathParts) {
        pathString += pathPart;
        pathString += '/';
    }
    if (!_pathParts.empty()) {
        pathString.pop_back();
    }
    return pathString;
}

size_t Path::getElementsCount() const {
    return _pathParts.size();
}

std::string Path::getBackPart() const {
    return _pathParts.back();
}

Path Path::withoutBackPart() const {
    std::vector<std::string> parts = _pathParts;//tu dac jekies sprawdzenie czy jest ten part
    parts.pop_back();
    return Path(parts);
}

Path Path::withBackPart(const std::string& part) const {
    std::vector<std::string> parts = _pathParts;
    parts.push_back(part);
    return Path(parts);
}