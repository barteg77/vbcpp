#include "Editor/Editor.h"

#include "Utils/Logger.h"
#include "Game/GameConfig.h"


int main()
{
	//Editor editor;

	//editor.run();

	Logger::init(LL_DEBUG, true, true, "logEditor.txt");
	GameConfig::getInstance().loadGameConfig("game.xml"); // loadr esource repos

	vbEditor::createWindow();
	vbEditor::initializeEngineSubsystems();

	vbEditor::run();

	vbEditor::shutDown();

	Logger::destroy();

	return 0;
}