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

    Path(const std::vector<std::string>& pathParts);//pathParts must be normalized like Path.pathParts
public:
    Path(const std::string& path);
    std::string getString() const;
    size_t getElementsCount() const;
    std::string getBackPart() const;
    Path withoutBackPart() const;
    Path withBackPart(const std::string& part) const;
};
#endif // PATH_H_INCLUDED