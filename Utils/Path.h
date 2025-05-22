#ifndef PATH_H_INCLUDED
#define PATH_H_INCLUDED
//#include <algorithm>
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <vector>
#include <string>

/**
 * @brief Case-insensitive normalized file path
 */
class Path
{
    std::vector<std::string> _pathParts;

    Path(const std::vector<std::string>& pathParts);//pathParts must be normalized like Path.pathParts
public:
    /**
     * @brief Construct Path according to path represented by provided string.
     * @param path non-normalized path string, "..",  multiple "/", "/" mixed
     * with "\" allowed
     */
    Path(const std::string& path);
    
    /**
     * @brief Get string representing path
     * @return string representation of Path, all characters lowercase,
     * directories' names separated with single "/"
     */
    std::string getString() const;

    /**
     * @brief Get count of elements (dirnames + filename) in this Path
     */
    size_t getElementsCount() const;

    /**
     * @brief Get back part of path
     */
    std::string getBackPart() const;

    /**
     * @brief Get Path object, same as this, but without the back part of path.
     */
    Path withoutBackPart() const;

    /**
     * @brief Get Path object, same as this, but with additional part at back.
     */
    Path withBackPart(const std::string& part) const;
};
#endif // PATH_H_INCLUDED