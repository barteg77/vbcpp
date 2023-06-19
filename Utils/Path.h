#ifndef PATH_H_INCLUDED
#define PATH_H_INCLUDED
//#include <algorithm>
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <vector>
#include <string>
class Path
{
    // This is case-insensitive file path, all characters are stored (and returned) LOWERCASE.
    std::vector<std::string> _pathParts;

public:
    Path(const std::vector<std::string>& pathParts)
    : _pathParts(pathParts) {}

    Path(const std::string& path) {
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

    /*std::vector<std::string> getParts() const {//     & ????
        return _pathParts;
    }*/

    std::string getString() const {
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

    size_t getElementsCount() const {
        return _pathParts.size();
    }

    std::string getBackPart() const {
        return _pathParts.back();
    }

    /*void dropBackPart() {
        _pathParts.pop_back();
    }

    void pushBackPart(const std::string& part) {
        _pathParts.push_back(part);
    }*/

    Path withoutBackPart() const {
        std::vector<std::string> parts = _pathParts;//tu dac jekies sprawdzenie czy jest ten part
        parts.pop_back();
        return Path(parts);
    }

    Path withBackPart(const std::string& part) const {
        std::vector<std::string> parts = _pathParts;
        parts.push_back(part);
        return Path(parts);
    }
};
#endif // PATH_H_INCLUDED