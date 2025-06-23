#include "ImGuiWindow.h"


VbImGuiWindow::VbImGuiWindow(SceneManager* sceneManager, bool isOpen)
    : _sceneManager(sceneManager), _isOpen(isOpen)
{

}

VbImGuiWindow::~VbImGuiWindow()
{
}


bool* VbImGuiWindow::getOpenFlagPointer()
{
    return &_isOpen;
}


void VbImGuiWindow::draw()
{
    if (_isOpen)
    {
        drawWindow();
    }
}
