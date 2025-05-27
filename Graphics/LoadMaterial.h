#ifndef LOADMATERIAL_H_INCLUDED
#define LOADMATERIAL_H_INCLUDED


#include "Material.h"

#include "../Utils/Strings.h"
#include "../Utils/Helpers.hpp"
#include "../Utils/ResourceManager.h"


namespace tinyxml2
{
	class XMLDocument;
}


// XML MATERIAL FILE DEFINITIONS
extern const char* XML_MATERIAL_ROOT;
extern const char* XML_MATERIAL_ELEMENT;
extern std::string matFilenamePostfix;
// XML END


// loads material data from xml file
class MaterialLoader
{
    private:
		tinyxml2::XMLDocument* _xmlFile;

        Material* loadMaterial(tinyxml2::XMLElement* materialElement, const std::string& materialName, const std::string& texPath);

    public:
        MaterialLoader();
        ~MaterialLoader();

        void openFile(const char* fileName);
        void closeFile();

        /**
         * @brief texturesDirectory name or empty string if not present
         */
        std::string getTexturesDirectoryName();

        Material* loadMaterial(const std::string& materialName, const std::string& texPath);
        void loadAllMaterials(std::vector<Material*>& outMaterials, std::string texPath);

        static std::string createMaterialFileName(const std::string& modelFileName)
        {
            std::string xmlFileName = modelFileName;
            xmlFileName.erase(xmlFileName.size() - 4, 4);
            xmlFileName += matFilenamePostfix;

            return xmlFileName;
        }

};


#endif // LOADMATERIAL_H_INCLUDED
