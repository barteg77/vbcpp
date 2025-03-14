#include "ResourceManager.h"

#include "FilesHelper.h"
#include "../Graphics/LoadTexture.h"
#include "../Graphics/LoadShader.h"
#include "../Graphics/StaticModelLoader.h"
#include "../Graphics/AnimatedModelLoader.h"
#include "../Graphics/AnimationLoader.h"
#include "../GUI/FontLoader.h"
#include "../Game/Directories.h"
#include "../Game/GameConfig.h"
#include "Logger.h"
#include "ResourceId.h"
#include "ResourceRepo.h"
#include "ResourceLocation.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>


static std::unique_ptr<ResourceManager> rsInstance;

const ResourceId ResourceManager::DEFAULT_WHITE_TEXTURE_RESOURCE_ID = ResourceId::create<RT_TEXTURE>(std::string(".defaultTexture"));

ResourceManager::ResourceManager()
{
    LOG_INFO("ResourceManager: Konstruktor");

	addResourceRepo(ResourceRepo("base", "."));
}


ResourceManager::~ResourceManager()
{
    LOG_INFO("ResourceManager: Destruktor");

}


ResourceManager& ResourceManager::getInstance()
{
    if( !rsInstance )
        rsInstance = std::unique_ptr<ResourceManager>(new ResourceManager);

    return* rsInstance;
}


Resource* ResourceManager::findResource(const ResourceId& resourceId)
{
// Sprawdzamy czy zasob juz istnieje
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getResourceId() == resourceId )
        {
            LOG_INFO("Resource istnieje. Zwracam istniejacy zasob: " + (*it)->getResourceId().getDebugString());
            std::unique_ptr<Resource>& res = *it;

            return res.get();
        }
    }

    return nullptr;
}

ResourceRepo* ResourceManager::findRepoOfResource(const ResourceId& resourceId)
{
    LOG_DEBUG("Finding repository of resource "+resourceId.getDebugString()+" files...");
    auto it = std::find_if(_resourceRepos.begin(), _resourceRepos.end(),
                           [&resourceId] (const std::unique_ptr<ResourceRepo>& resourceRepo)
                           {
                               LOG_DEBUG("Checking for resource in repo " + resourceRepo->getPath());
                               return resourceRepo->doResourceExists(resourceId);
                           });
    if (it == _resourceRepos.end()){
        LOG_ERROR("Resource "+resourceId.getDebugString()+" file(s) NOT FOUND in any repository!");
        return nullptr;
    }
    LOG_DEBUG("Resource "+resourceId.getDebugString()+" file(s) found in repository: "+it->get()->getDebugString());
    return it->get();

}

ResourceLocation ResourceManager::findResourceLocation(const ResourceId& resourceId)
{
    return ResourceLocation(resourceId, *findRepoOfResource(resourceId));
}

std::string ResourceManager::realPath(const std::string& pseudoId)
{
    LOG_DEBUG("Getting realPath by ResourceManager (not loading the resource");
    const ResourceId resourceId = ResourceId::create<RT_OTHER>(pseudoId);
    return ResourceManager::getInstance().findResourceLocation(resourceId).getPath();
}

// Ładowanie tektur
RTexture2D* ResourceManager::loadTexture(const ResourceId& resourceId, bool useCompression, bool mipmapping, bool useAnisotropicFiltering)
{
    Resource* res = findResource(resourceId);
    if (res != 0)
    {
        RTexture2D* tex = dynamic_cast<RTexture2D*>(res);
        return tex;
    }
    int width, height;
    //GLuint tID = ::loadTexture(path.c_str(), &width, &height, true);
	bool textureCompression = useCompression && GameConfig::getInstance().textureCompression;
    RTexture2D* texture = ::loadTexture(resourceId, getPath(resourceId), textureCompression, mipmapping);

    if ( texture )
    {
		bool anisotropic = useAnisotropicFiltering && GameConfig::getInstance().anisotropicFiltering;
		float anisotropySamples = GameConfig::getInstance().anisotropySamples;
		if (anisotropic)
			texture->setAnisotropyFiltering(anisotropic, anisotropySamples);

        //std::unique_ptr<RTexture> tex (new RTexture(path, tID, TT_2D, glm::uvec2(width, height)));
        std::unique_ptr<RTexture> tex (texture);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + tex.get()->getResourceId().getDebugString());

        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Dlatego kopiuje sobie ID textury przez przesunieciem wskaznika do listy
        //GLuint texID = tex->getID();
        _resources.push_back(std::move(tex));


        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Musialem odwolac sie do utworzonej tekstury poprzez iterator do ostatniego elementu na liscie (nowa tekstura jest zawsze wrzucana na koniec listy)
        std::list<std::unique_ptr<Resource>>::iterator it = _resources.end();
        std::unique_ptr<Resource>& res = *(--it);

        RTexture2D* t = dynamic_cast<RTexture2D*>(res.get());


        //std::cout << "Texture ID: " << tID << std::endl;

        return t;
    }

    return 0;
}


RTextureCubeMap* ResourceManager::loadTextureCubeMap(const ResourceId& resourceId)
{
    //ResourceId resourceId(std::vector<std::string>(fileNames, fileNames+6));

    Resource* res = findResource(resourceId);
    if (res != nullptr)
    {
        RTextureCubeMap* tex = dynamic_cast<RTextureCubeMap*>(res);
        return tex;
    }
    // Zasob nie istnieje

    // prepare prepare files' paths
    const std::vector<std::string> idParts(resourceId.getIdParts());
    std::vector<std::string> filesNames;
    filesNames.reserve(idParts.size());
    for (int i=0;i<idParts.size();i++) {
        filesNames.push_back(getPath(resourceId, i));
    }
    
    RTextureCubeMap* texture = ::loadTextureCubeMap(resourceId, filesNames, true);

    if ( texture )
    {
        //std::unique_ptr<RTexture> tex (new RTexture(path, tID, TT_2D, glm::uvec2(width, height)));
        std::unique_ptr<RTexture> tex (texture);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + tex.get()->getResourceId().getDebugString());

        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Dlatego kopiuje sobie ID textury przez przesunieciem wskaznika do listy
        //GLuint texID = tex->getID();
        _resources.push_back(std::move(tex));


        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Musialem odwolac sie do utworzonej tekstury poprzez iterator do ostatniego elementu na liscie (nowa tekstura jest zawsze wrzucana na koniec listy)
        std::list<std::unique_ptr<Resource>>::iterator it = _resources.end();
        std::unique_ptr<Resource>& res = *(--it);

        RTextureCubeMap* t = dynamic_cast<RTextureCubeMap*>(res.get());


        //std::cout << "Texture ID: " << tID << std::endl;

        return t;
    }

    return 0;
}


void ResourceManager::reloadTexture(RTexture2D* texture)
{
    ::loadTexture(texture->getResourceId(), getPath(texture->getResourceId()), texture->isCompressed(), true, texture);
}


void ResourceManager::reloadTexture(const ResourceId& resourceId)
{
    Resource* texture = findResource(resourceId);
    if (texture != NULL)
    {
        reloadTexture(dynamic_cast<RTexture2D*>(texture));
    }
}


void ResourceManager::reloadTexture(RTextureCubeMap* texture)
{
    /*std::string fileNames[6];
    istringstream stream(texture->getPath());
    std::string s;
    int index = 0;
    while (getline(stream, s, ';')) {
        fileNames[index++] = s;
    }

    ::loadTextureCubeMap(fileNames, texture->getPath().c_str(), true, texture);*/
}


void ResourceManager::reloadAllTextures()
{
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getType() == RT_TEXTURE )
        {
            std::unique_ptr<Resource>& res = *it;

            RTexture* texture = dynamic_cast<RTexture*>(res.get());
            if (texture->getTextureType() == TT_2D)
                reloadTexture(dynamic_cast<RTexture2D*>(texture));
            else if (texture->getTextureType() == TT_CUBE)
                reloadTexture(dynamic_cast<RTextureCubeMap*>(texture));
        }
    }
}


RTexture2D* ResourceManager::loadDefaultWhiteTexture()
{
    Resource* res = findResource(DEFAULT_WHITE_TEXTURE_RESOURCE_ID);
    if (res != 0)
    {
        RTexture2D* tex = dynamic_cast<RTexture2D*>(res);
        return tex;
    }

    // Zasob nie istnieje
    glm::uvec2 size(2, 2);
	bool textureCompression = GameConfig::getInstance().textureCompression;
    RTexture2D* texture = RTexture2D::createWhiteTexture(DEFAULT_WHITE_TEXTURE_RESOURCE_ID, size, textureCompression);

    if ( texture )
    {
        //std::unique_ptr<RTexture> tex (new RTexture(path, tID, TT_2D, glm::uvec2(width, height)));
        std::unique_ptr<RTexture> tex (texture);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + tex.get()->getResourceId().getDebugString());

        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Dlatego kopiuje sobie ID textury przez przesunieciem wskaznika do listy
        //GLuint texID = tex->getID();
        _resources.push_back(std::move(tex));


        // Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
        // Musialem odwolac sie do utworzonej tekstury poprzez iterator do ostatniego elementu na liscie (nowa tekstura jest zawsze wrzucana na koniec listy)
        std::list<std::unique_ptr<Resource>>::iterator it = _resources.end();
        std::unique_ptr<Resource>& res = *(--it);

        RTexture2D* t = dynamic_cast<RTexture2D*>(res.get());


        //std::cout << "Texture ID: " << tID << std::endl;

        return t;
    }

    return 0;
}


RTexture2D* ResourceManager::loadOneColorTexture(glm::vec4 color)
{
	std::string name = "texture(" + toString(color.r) + "," + toString(color.g) + "," + toString(color.b) + "," + toString(color.a) + ")";
	const ResourceId resourceId = ResourceId::create<RT_TEXTURE>(name);

	Resource* res = findResource(resourceId);
	if (res != nullptr)
	{
		RTexture2D* tex = dynamic_cast<RTexture2D*>(res);
		return tex;
	}

	// Zasob nie istnieje
	bool textureCompression = GameConfig::getInstance().textureCompression;
	RTexture2D* texture = RTexture2D::createOneColorTexture(resourceId, color, textureCompression);

	if (texture)
	{
		//std::unique_ptr<RTexture> tex (new RTexture(path, tID, TT_2D, glm::uvec2(width, height)));
		std::unique_ptr<RTexture> tex(texture);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + tex.get()->getResourceId().getDebugString());

		// Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
		// Dlatego kopiuje sobie ID textury przez przesunieciem wskaznika do listy
		//GLuint texID = tex->getID();
		_resources.push_back(std::move(tex));


		// Poniewaz std::move przenosi wartosc z pamieci obiektu 'tex' do pamiêci listy '_resources', nie mozna wiecej odwolac sie do obiektu 'tex'
		// Musialem odwolac sie do utworzonej tekstury poprzez iterator do ostatniego elementu na liscie (nowa tekstura jest zawsze wrzucana na koniec listy)
		std::list<std::unique_ptr<Resource>>::iterator it = _resources.end();
		std::unique_ptr<Resource>& res = *(--it);

		RTexture2D* t = dynamic_cast<RTexture2D*>(res.get());


		//std::cout << "Texture ID: " << tID << std::endl;

		return t;
	}

	return 0;
}


// Ładowanie shaderów
RShader* ResourceManager::loadShader(const ResourceId& resourceId)
{
    Resource* res = findResource(resourceId);
    if (res != 0)
    {
        RShader* shdr = dynamic_cast<RShader*>(res);
        return shdr;
    }

    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + resourceId.getDebugString());

    const std::vector<std::string> idParts = resourceId.getIdParts();
    assert(idParts.size() == 2);// todo jak bedzie ResourceType w ResourceId i tam assert to zrobic assert na resource type
    const std::string& vertexShaderFileName = getPath(resourceId, 0);
    const std::string& fragmentShaderFileName = getPath(resourceId, 1);
    
    std::unique_ptr<Resource> shader ( new RShader(resourceId, ShaderLoader::loadShader(vertexShaderFileName,
                                                                                        fragmentShaderFileName,
                                                                                        resourceId.getDefines(),
                                                                                        resourceId.getConstants())));

    RShader* s = dynamic_cast<RShader*>( shader.get() );

    if ( s )
    {
        _resources.push_back(std::move(shader));
        return s;
    }
    else
        return 0;
}


void ResourceManager::reloadShader(RShader* shader)
{
    const ResourceId& resourceId(shader->getResourceId());
    //to jest copy paste z load shadera ale kiedys zrobie dobrze
    const std::vector<std::string> idParts = resourceId.getIdParts();
    assert(idParts.size() == 2);// todo jak bedzie ResourceType w ResourceId i tam assert to zrobic assert na resource type
    const std::string& vertexShaderFileName = getPath(resourceId, 0);
    const std::string& fragmentShaderFileName = getPath(resourceId, 1);
    shader->setNewShader(ShaderLoader::loadShader(vertexShaderFileName,
                                                  fragmentShaderFileName,
                                                  resourceId.getDefines(),
                                                  resourceId.getConstants()));
}


void ResourceManager::reloadAllShaders()
{
    std::list<std::unique_ptr<Resource>>::iterator it;
    for ( it = _resources.begin(); it != _resources.end(); ++it)
    {
        if ( (*it)->getType() == RT_SHADER )
        {
            std::unique_ptr<Resource>& res = *it;

            reloadShader(dynamic_cast<RShader*>(res.get()));
        }
    }
}


RStaticModel* ResourceManager::loadModelWithHierarchy(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing)
{
    Resource* res = findResource(resourceId);
    if (res != 0)
    {
        RStaticModel* model = dynamic_cast<RStaticModel*>(res);
        return model;
    }

    StaticModelLoader loader(normalsSmoothing);
    std::unique_ptr<RStaticModel> model(loader.loadModelWithHierarchy(resourceId, getPath(resourceId), texturePath) );
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + model.get()->getResourceId().getDebugString());

    RStaticModel* m = dynamic_cast<RStaticModel*>( model.get() );

    if ( m )
    {
        _resources.push_back(std::move(model));
        return m;
    }
    else
        return 0;
}


/*RStaticModel* ResourceManager::loadModelWithHierarchyOnlyNode(std::string path, std::string texturePath, std::string nodeToLoadName, Transform& loadedNodeTransformInModel, bool normalsSmoothing)
{
    std::string pathForResourceManager = path + "+" + nodeToLoadName;

    Resource* res = findResource(pathForResourceManager);
    if (res != 0)
    {
        RStaticModel* model = dynamic_cast<RStaticModel*>(res);
        return model;
    }

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(path))
		path = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    StaticModelLoader loader(normalsSmoothing);
    std::unique_ptr<RStaticModel> model( loader.loadModelWithHierarchyOnlyNode(path, texturePath, nodeToLoadName, loadedNodeTransformInModel) );
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + model.get()->getPath());

    RStaticModel* m = dynamic_cast<RStaticModel*>( model.get() );

    if ( m )
    {
        _resources.push_back(std::move(model));
        return m;
    }
    else
        return 0;
}*/


/*void ResourceManager::loadModelWithHierarchyOnlyNodes(std::string path, std::string texturePath, std::vector<std::string> nodesToLoadNames,
                                                      std::vector<Transform>& loadedNodesTransformsInModel, std::vector<RStaticModel*>& loadedNodes,
													  bool normalsSmoothing)
{
    loadedNodesTransformsInModel.resize(nodesToLoadNames.size());
    loadedNodes.resize(nodesToLoadNames.size());

    StaticModelLoader loader(normalsSmoothing);

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(path))
		path = _alternativeResourcePath + path;
#endif // DEVELOPMENT_RESOURCES

    for (int i = 0; i < nodesToLoadNames.size(); ++i)
    {
        RStaticModel* model = loader.loadModelWithHierarchyOnlyNode(path, texturePath, nodesToLoadNames[i], loadedNodesTransformsInModel[i]);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + path);

        loadedNodes[i] = model;
    }
}*/


RStaticModel* ResourceManager::loadModel(const ResourceId& resourceId, std::string texturePath, bool normalsSmoothing)
{
    Resource* res = findResource(resourceId);
    if (res != 0)
    {
        RStaticModel* model = dynamic_cast<RStaticModel*>(res);
        return model;
    }

    StaticModelLoader loader(normalsSmoothing);
    std::unique_ptr<RStaticModel> model( loader.loadModel(resourceId, getPath(resourceId), texturePath));
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + model.get()->getResourceId().getDebugString());

    RStaticModel* m = dynamic_cast<RStaticModel*>( model.get() );

    if ( m )
    {
        _resources.push_back(std::move(model));
        return m;
    }
    else
        return 0;
}


RAnimatedModel* ResourceManager::loadAnimatedModel(const ResourceId& resourceId, const std::string& texturePath, const std::unordered_map<std::string, BoneInfo*>& boneInfosFromExistingModel/* = {}*/)
{
    Resource* res = findResource(resourceId);
    if (res != 0)
    {
        RAnimatedModel* model = dynamic_cast<RAnimatedModel*>(res);
        return model;
    }

    AnimatedModelLoader loader;
    std::unique_ptr<RAnimatedModel> model(loader.loadAnimatedModelWithHierarchy(resourceId, getPath(resourceId), texturePath, boneInfosFromExistingModel));
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + model.get()->getResourceId().getDebugString());

    RAnimatedModel* m = dynamic_cast<RAnimatedModel*>(model.get());

    if (m)
    {
        _resources.push_back(std::move(model));
        return m;
    }
    else
        return 0;
}


RAnimation* ResourceManager::loadAnimation(const ResourceId& resourceId)
{
    Resource* res = findResource(resourceId);
    if (res != 0)
    {
        RAnimation* animation = dynamic_cast<RAnimation*>(res);
        return animation;
    }

    AnimationLoader loader;
    std::unique_ptr<RAnimation> animation(loader.loadAnimation(findResourceLocation(resourceId)));
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + animation.get()->getResourceId().getDebugString());

    RAnimation* a = dynamic_cast<RAnimation*>(animation.get());

    if (a)
    {
        _resources.push_back(std::move(animation));
        return a;
    }
    else
        return 0;
}


RFont* ResourceManager::loadFont(const ResourceId& resourceId)
{
    //FontLoader::createFontResourceName(path.c_str(), pixelSize)///////////////////////////     <----- !!!
    Resource* res = findResource(resourceId);
    if (res != 0)
    {
        RFont* font = dynamic_cast<RFont*>(res);
        return font;
    }

    FontLoader loader;

    RFont* mTemp = loader.loadFont(resourceId, getPath(resourceId));

    if (mTemp)
    {
        std::unique_ptr<RFont> font(mTemp);
        LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + font.get()->getResourceId().getDebugString());

        RFont* f = dynamic_cast<RFont*>( font.get() );

        if ( f )
        {
            _resources.push_back(std::move(font));
            return f;
        }
        else
            return 0;
    }

    return 0;
}


RSound* ResourceManager::loadSound(const ResourceId& resourceId)
{
    Resource* res = findResource(resourceId);
    if (res != 0)
    {
        RSound* sound = dynamic_cast<RSound*>(res);
        return sound;
    }

    std::unique_ptr<RSound> sound( ::loadSound(resourceId, getPath(resourceId)) );
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + sound.get()->getResourceId().getDebugString());

    RSound* s = dynamic_cast<RSound*>( sound.get() );

    if ( s )
    {
        _resources.push_back(std::move(sound));
        return s;
    }
    else
        return 0;
}
RObject* ResourceManager::loadRObject(const std::string& name)
{
  return loadRObject(ResourceId::create<RT_OBJECT>(GameDirectories::OBJECTS + name + "/"), name);
}

RObject* ResourceManager::loadRObject(const ResourceId& resourceId, const std::string& originalName)
{
	Resource* res = findResource(resourceId);
	if (res != 0)
	{
		RObject* object = dynamic_cast<RObject*>(res);
		return object;
	}

	std::unique_ptr<RObject> object(RObjectLoader::loadObject(findResourceLocation(resourceId), originalName));
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + object.get()->getResourceId().getDebugString());

	RObject* o = dynamic_cast<RObject*>(object.get());

	if (o)
	{
		_resources.push_back(std::move(object));
		return o;
	}
	else
		return 0;
}


RRoadProfile* ResourceManager::loadRoadProfile(const std::string& name)
{
  return loadRoadProfile(ResourceId::create<RT_ROAD_PROFILE>(GameDirectories::ROAD_PROFILES + name + "/"));
}

RRoadProfile* ResourceManager::loadRoadProfile(const ResourceId& resourceId)
{
	Resource* res = findResource(resourceId);
	if (res != 0)
	{
		RRoadProfile* object = dynamic_cast<RRoadProfile*>(res);
		return object;
	}

	std::unique_ptr<RRoadProfile> object(RoadProfileLoader::loadRoadProfile(resourceId, getPath(resourceId)));
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + object.get()->getResourceId().getDebugString());

	RRoadProfile* o = dynamic_cast<RRoadProfile*>(object.get());

	if (o)
	{
		_resources.push_back(std::move(object));
		return o;
	}
	else
		return 0;
}


RDisplayFont* ResourceManager::loadDisplayFont(const std::string& name)
{
  return loadDisplayFont(ResourceId::create<RT_DISPLAY_FONT>(GameDirectories::DISPLAYS + name + "/"));
}

RDisplayFont* ResourceManager::loadDisplayFont(const ResourceId& resourceId)
{
    Resource* res = findResource(resourceId);
	if (res != 0)
	{
		RDisplayFont* object = dynamic_cast<RDisplayFont*>(res);
		return object;
	}

	std::unique_ptr<RDisplayFont> object(new RDisplayFont(findResourceLocation(resourceId)));
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + object.get()->getResourceId().getDebugString());

	RDisplayFont* o = dynamic_cast<RDisplayFont*>(object.get());

	if (o)
	{
		_resources.push_back(std::move(object));
		return o;
	}
	else
		return 0;
}


RMaterialsCollection* ResourceManager::loadMaterialsCollection(const ResourceId& resourceId)
{
    Resource* res = findResource(resourceId);
    if (res != 0)
    {
        RMaterialsCollection* materialsCollection = dynamic_cast<RMaterialsCollection*>(res);
        return materialsCollection;
    }

    RMaterialsCollection* temp = new RMaterialsCollection(resourceId);

    MaterialLoader materialLoader;
    std::string path = findResourceLocation(resourceId).getPath();
    std::string id_string_dir = FilesHelper::getPathToDirectoryFromFileName(resourceId.getIdString(0));
    materialLoader.openFile(path.c_str());

    materialLoader.loadAllMaterials(temp->getMaterials(), FilesHelper::getPathToDirectoryFromFileName(id_string_dir));

    materialLoader.closeFile();

    std::unique_ptr<RMaterialsCollection> materialsCollection(temp);
    LOG_INFO("Resource nie istnieje. Tworzenie nowego zasobu... " + materialsCollection.get()->getResourceId().getDebugString());

    RMaterialsCollection* mc = dynamic_cast<RMaterialsCollection*>(materialsCollection.get());

    if (mc)
    {
        _resources.push_back(std::move(materialsCollection));
        return mc;
    }
    else
        return 0;
}

void ResourceManager::addResourceRepo(const ResourceRepo& resourceRepo)
{
    _resourceRepos.push_back(std::unique_ptr<ResourceRepo> (new ResourceRepo (resourceRepo)));
}

std::string ResourceManager::getPath(const ResourceId& resourceId, const size_t partIdx) {
    ResourceLocation resourceLocation = findResourceLocation(resourceId);
    const ResourceType& rt(resourceId.getResourceType());
    const std::string& idPart(resourceId.getIdParts().at(partIdx));
    std::string path_str;
        if (rt == ResourceType::RT_OBJECT) {
            path_str = idPart + "object.xml";
        } else if (rt == ResourceType::RT_ROAD_PROFILE) {// to sie ucywilizuje jak sie wydzieli pochodne ResourceRepo tego typu
            path_str = idPart + "profile.xml";
        } else {
            path_str = idPart;// kiedys sie zrobi zebt tegi nie kopiowac
        }
    std::string path(resourceLocation.repo._filesystemHelper.get()->getKnownActualFilesystemFilepath(Path(path_str)));
    return path;
}
