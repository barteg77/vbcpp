#ifndef IMGUIWINDOW_H_INCLUDED
#define IMGUIWINDOW_H_INCLUDED


#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../../Graphics/Renderer.h"

#include "../../Scene/SceneManager.h"


class VbImGuiWindow
{
    protected:
        SceneManager* _sceneManager;

        bool _isOpen;

        virtual void drawWindow() = 0;

    public:
		VbImGuiWindow(SceneManager* sceneManager, bool isOpen = true);
        virtual ~VbImGuiWindow();

        bool* getOpenFlagPointer();

        void draw();

};


#endif // IMGUIWINDOW_H_INCLUDED
