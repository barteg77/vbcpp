#include "RMaterialsCollection.h"

#include "Material.h"


RMaterialsCollection::RMaterialsCollection(const ResourceId& resourceId)
	: Resource(RT_MATERIALS_COLLECTION, resourceId)
{

}


RMaterialsCollection::~RMaterialsCollection()
{
	for (int i = 0; i < _materials.size(); ++i)
	{
		delete _materials[i];
	}
}


std::vector<Material*>& RMaterialsCollection::getMaterials()
{
	return _materials;
}

Material* RMaterialsCollection::getMaterial(const std::string& name) {
	for (Material* material : _materials) {
		if (material->name == name) {
			return material;
		}
	}
	return nullptr;
}