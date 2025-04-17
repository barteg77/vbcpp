#include "RObject.h"
#include "ResourceId.h"


RObject::RObject(const ResourceId& resourceId, const std::string& author, const std::string& name, const std::string& comment, const std::string& originalName)
	: Resource(RT_OBJECT, resourceId),
	_author(author), _name(name), _comment(comment),
	_originalName(originalName)
{

}


std::vector<std::unordered_map<std::string, std::string>>& RObject::getComponents()
{
	return _components;
}


const std::string& RObject::getAuthor()
{
	return _author;
}


const std::string& RObject::getName()
{
	return _name;
}


const std::string& RObject::getComment()
{
	return _comment;
}


const std::string& RObject::getOriginalName()
{
	return _originalName;
}
