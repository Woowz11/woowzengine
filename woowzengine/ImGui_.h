#pragma once
#include <sol/sol.hpp>
#include "l_Color.h"

using namespace std;

void CreateImGuiWindow(string id, string title);
void CreateImGuiElement(string id, string window, string type);
void SetImGuiElementText(string id, string text);
void SetImGuiElementColor(string id, l_Color color);
void SetImGuiElementEvent(string id, sol::function func);
void ImGuiRender(string windowid);
void ImGuiStyle_();