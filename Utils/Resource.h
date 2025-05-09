#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED

#include <string>
using std::string;

#include "RefCounter.h"
#include "ResourceId.h"

class Resource
{
    public:
        Resource(ResourceType type, const ResourceId& resourceId)
        : _type(type), _resourceId(resourceId) {}

        virtual ~Resource() {}

        ResourceId getResourceId() const
        { return _resourceId; }

        std::string getPath() const// use ONLY! in Editor/Windows/ObjectPropertiesWindow.cpp
        {
            assert(_resourceId.getIdPartsCount() == 1);
            return _resourceId.getIdString(0);
        }

        ResourceType getType() const
        { return _type; }

    protected:
        ResourceId _resourceId;

    private:
        ResourceType _type;
};


#endif // RESOURCE_H_INCLUDED
