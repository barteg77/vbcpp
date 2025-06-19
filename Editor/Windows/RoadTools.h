#pragma once

#include <vector>

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui.h>

#include "../../Scene/SceneManager.h"

namespace ImGui
{
	void RoadProfileGraph(const char* label, std::vector<ImVec2>& values, float scale_min, float scale_max, ImVec2 frame_size);

	void DrawRoadShape(std::vector<RoadSegment>& segments);
}