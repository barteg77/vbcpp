#ifndef MATERIALSCOLLECTION_H_INCLUDED
#define MATERIALSCOLLECTION_H_INCLUDED


#include <vector>

#include "../Utils/Resource.h"


class Material;


class RMaterialsCollection : public Resource
{
	private:
		std::vector<Material*> _materials;

	public:
		RMaterialsCollection(const ResourceId& resourceId);
		~RMaterialsCollection();

		std::vector<Material*>& getMaterials();
};


#endif // MATERIALSCOLLECTION_H_INCLUDED