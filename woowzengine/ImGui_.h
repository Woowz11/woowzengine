#pragma once
#include <sol/sol.hpp>
#include <vector>
#include "l_Color.h"

using namespace std;

void CreateImGuiWindow(string id, string title, bool visible);
void CreateImGuiElement(string id, string window, string type);
void SetImGuiElementText(string id, string text);
void SetImGuiElementColor(string id, l_Color color);
void SetImGuiElementEvent(string id, sol::function func);
void SetImGuiElementStringValue(string id, string str);
void SetImGuiElementConnect(string id, bool b);
void SetImGuiElementTooltip(string id, string str);
void SetImGuiElementPoint(string id, bool b);
void SetImGuiFontSize(float size);
void SetImGuiWindowActive(string id, bool b);
void SetImGuiElementValue(string id, vector<variant<string, double, bool, l_Color>> val1, vector<variant<string, double, bool, l_Color>> val2, vector<variant<string, double, bool, l_Color>> val3, vector<variant<string, double, bool, l_Color>> val4);
variant<string, vector<string>, double, vector<double>, bool, l_Color> GetImGuiElementValue(string id);
void ImGuiRender(string windowid);
void ImGuiStyle_();