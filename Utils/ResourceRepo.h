#ifndef RESOURCEREPO_H_INCLUDED
#define RESOURCEREPO_H_INCLUDED


#include <string>
#include "ResourceRepoFilesystemHelper.h"

class ResourceRepo final
{
    friend class ResourceManager;// to jest tylko na chwile dopoki loady nie są przeniesione do ResourceRepo

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
private:
    std::string getActualFilesystemFilepath(Path filePath) {
        return _filesystemHelper->getActualFilesystemFilepath(filePath);
    }

public:
    bool doResourceExists(const ResourceId& resourceId) {
        const std::string es("");
        for (const std::string& idPart : resourceId.getIdParts()) {
            ResourceType rt = resourceId.getResourceType();
            std::string path_str;
            if (rt == ResourceType::RT_OBJECT) {
                path_str = idPart + "object.xml";
            } else if (rt == ResourceType::RT_ROAD_PROFILE) {// to sie ucywilizuje jak sie wydzieli pochodne ResourceRepo tego typu
                path_str = idPart + "profile.xml";
            } else {
                path_str = idPart;// kiedys sie zrobi zebt tegi nie kopiowac
            }
            if (getActualFilesystemFilepath(Path(path_str)) == es) {
                return false;
            }
        }
        return true;
    }
};


#endif // RESOURCEREPO_H_INCLUDED