#ifndef GAMECONFIG_H_INCLUDED
#define GAMECONFIG_H_INCLUDED


#include <string>

#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
#include <vector>
#include <assert.h>
using namespace tinyxml2;


enum GameMode
{
	GM_GAME,
	GM_EDITOR
};


class GameConfig
{
    public:
    enum class RepoType {
        Generics,
        Native,
        Dummy
    };

    struct RepoDefinition {
        RepoDefinition(const RepoType type,
                    const std::string& name,
                    const std::string& path)
        : _type(type)
        , _name(name)
        , _path(path)
        {
            if (_type == RepoType::Native) {
                assert(!_name.empty());
                assert(!_path.empty());
            }
        }
        RepoType _type;
        std::string _name;
        std::string _path;
    };

    private:
        static GameConfig* instance;

        GameConfig()
            : windowWidth(1024), windowHeight(768),
            fullscreenMode(0), verticalSync(false), hdrQuality(16), msaaAntialiasing(false), msaaAntialiasingLevel(8), isFxaaEnabled(true), fxaaQuality(10), isBloomEnabled(true),
			isShadowmappingEnable(true), shadowmapSize(1024),
			isGrassEnable(true), grassRenderingDistance(30.0f), isMirrorsEnabled(true), mirrorSize(256.0f), mirrorRenderingDistance(100.0f), mirrorRefreshDistance(5.0f),
			textureCompression(false), anisotropicFiltering(false), anisotropySamples(4.0f),
			pbrSupport(false), openGlDebugContext(false),
            loggerLevel(""), loggerConsoleOutput(false), loggerFileOutput(""),
            developmentMode(false), firstScene(""), useLoadingScreen(true),
			mode(GM_GAME)
        {}
        GameConfig(const GameConfig&) {}

    public:
        static GameConfig& getInstance()
        {
            if (instance == NULL)
                instance = new GameConfig;

            return *instance;
        }

        void loadGameConfig(const char* filename);
		void loadDevelopmentConfig(const char* filename);

		GameMode mode;

        int windowWidth;
        int windowHeight;
        std::string mapFile;
        std::string mapTexPath;
        std::string busModel;
        std::string busRepaint;
        std::string busConfiguration;

        std::vector<RepoDefinition> resourceRepos;

        int fullscreenMode;
		bool verticalSync;
		int hdrQuality;
        bool msaaAntialiasing;
        int msaaAntialiasingLevel;
        bool isFxaaEnabled;
        int fxaaQuality;
        bool isShadowmappingEnable;
        int shadowmapSize;
        bool isBloomEnabled;
        bool isGrassEnable;
		float grassRenderingDistance;
		bool isMirrorsEnabled;
        float mirrorSize;
		float mirrorRenderingDistance;
        float mirrorRefreshDistance;
		bool textureCompression;
		bool anisotropicFiltering;
		float anisotropySamples;

		bool pbrSupport;

        bool openGlDebugContext;

        std::string loggerLevel;
        bool loggerConsoleOutput;
        std::string loggerFileOutput;

        std::string language;

		bool developmentMode;
        std::string firstScene;
        bool useLoadingScreen;

};


#endif // GAMECONFIG_H_INCLUDED
