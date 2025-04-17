#ifndef RESOURCEREPO_H_INCLUDED
#define RESOURCEREPO_H_INCLUDED


#include <string>


class ResourceRepo final
{
	std::string _name;
    std::string _path;
public:
    explicit ResourceRepo(const std::string& name,
                          const std::string& path)
    : _name(name), _path(path) {}
    
    std::string getName() const { return _name; }
    std::string getPath() const { return _path; }
    std::string getDebugString() const
    { return "name:"+_name+" path:"+_path; }
};


#endif // RESOURCEREPO_H_INCLUDED