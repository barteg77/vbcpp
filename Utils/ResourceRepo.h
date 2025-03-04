#ifndef RESOURCEREPO_H_INCLUDED
#define RESOURCEREPO_H_INCLUDED


#include <string>
#include "ResourceRepoFilesystemHelper.h"

class ResourceRepo final
{
	std::string _name;
    std::string _path;
    std::shared_ptr<FilesystemHelper> _filesystemHelper;

public:
    explicit ResourceRepo(const std::string& name,
                          const std::string& path)
    : _name(name),
      _path(path),
      _filesystemHelper(new FilesystemHelper(path, *FilesHelper::getInstance()))
    {}
    
    std::string getName() const { return _name; }
    std::string getPath() const { return _path; }
    std::string getDebugString() const
    { return "name:"+_name+" path:"+_path; }

    // returns real (case sensitive) file path of given case-insensitive filepath if exists
    // returns empth string if wanted file doesn't exist
    std::string getActualFilesystemFilepath(Path filePath) {
        return _filesystemHelper->getActualFilesystemFilepath(filePath);
    }

    bool doResourceExists(const ResourceId& resourceId) {
        const std::string es("");
        for (const std::string& idPart : resourceId.getIdParts()) {
            if (getActualFilesystemFilepath(Path(resourceId.getResourceType() == RT_OBJECT ? idPart + "object.xml" : idPart)) == es) {
                return false;
            }
        }
        return true;
    }
};


#endif // RESOURCEREPO_H_INCLUDED