#define _CRT_SECURE_NO_WARNINGS

#include <unordered_map>
#include <sol/sol.hpp>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "Easyer.h"
#include "l_Color.h"
#include "ImGuiWindow_.h"
#include "ImGuiElement_.h"
#include "Base.h"

using namespace std;

vector<string> ElementTypes{"text","button","checkbox"};

unordered_map<string, ImGuiWindow_> Windows;
unordered_map<string, ImGuiElement_> Elements;

string ErrorElement = "tfgw3gsss#^@g eg12t g dsgsegkfk0wf03_#gef#tfqgeg#t 3 gwg @#g gg@#gsdg vegwe 3 g";

void CreateImGuiWindow(string id, string title, bool visible) {
	if (Windows.find(id) == Windows.end()) {
		ImGuiWindow_ IGW = ImGuiWindow_(id);
		IGW.title = title;
		IGW.active = visible;
		Windows[id] = IGW;
	}
	else {
		PE("ImGui window with this ID ['"+id+"'] already exists! CreateImGuiWindow('"+id+"','"+title+"')","L0043");
	}
}

int zindexs = 0;
unordered_map<int, string> zpos;
void CreateImGuiElement(string id, string window, string type) {
	if (Elements.find(id) == Elements.end()) {
		zindexs++;
		ImGuiElement_ IGE = ImGuiElement_(id, window, type, zindexs);
		Elements[id] = IGE;
		zpos[zindexs] = id;
		if (Windows.find(window) == Windows.end()) {
			PW("Selected window ['"+window+"'] was not found. CreateImGuiElements('" + id + "','" + window + "','" + type + "')","LW0043");
		}
	}
	else {
		PE("ImGui element with this ID ['" + id + "'] already exists! CreateImGuiElements('" + id + "','" + window + "','"+type+"')", "L0044");
	}
}

void AcceptWindow(ImGuiWindow_ IGE) {
	if (IGE.id != ErrorElement) {
		Windows[IGE.id] = IGE;
	}
}

ImGuiWindow_ GetWindow(string id) {
	if (Windows.find(id) != Windows.end()) {
		return Windows[id];
	}
	else {
		PE("ImGui window ['" + id + "'] was not found! GetWindow('" + id + "')", "E0044");
		return ImGuiWindow_(ErrorElement);
	}
}

void SetImGuiWindowActive(string id, bool b) {
	ImGuiWindow_ IGW = GetWindow(id);
	IGW.active = b;
	AcceptWindow(IGW);
}

void AcceptElement(ImGuiElement_ IGE) {
	if (IGE.id != ErrorElement) {
		Elements[IGE.id] = IGE;
	}
}

ImGuiElement_ GetElement(string id) {
	if (Elements.find(id) != Elements.end()) {
		return Elements[id];
	}
	else {
		PE("ImGui element ['"+id+"'] was not found! GetElement('"+id+"')","E0043");
		return ImGuiElement_(ErrorElement,"","error",0);
	}
}

void SetImGuiElementText(string id, string text) {
	ImGuiElement_ IGE = GetElement(id);
	IGE.text = text;
	AcceptElement(IGE);
}

void SetImGuiElementColor(string id, l_Color color) {
	ImGuiElement_ IGE = GetElement(id);
	IGE.color = color;
	AcceptElement(IGE);
}

void SetImGuiElementEvent(string id, sol::function func) {
	ImGuiElement_ IGE = GetElement(id);
	IGE.func = func;
	AcceptElement(IGE);
}

void SetImGuiElementPoint(string id, bool b) {
	ImGuiElement_ IGE = GetElement(id);
	IGE.hasbullet = b;
	AcceptElement(IGE);
}

void SetImGuiElementStringValue(string id, string str) {
	ImGuiElement_ IGE = GetElement(id);
	IGE.stringvalue = str;
	AcceptElement(IGE);
}

void SetImGuiElementConnect(string id, bool b) {
	ImGuiElement_ IGE = GetElement(id);
	IGE.sameline = b;
	AcceptElement(IGE);
}

void SetImGuiElementTooltip(string id, string str) {
	ImGuiElement_ IGE = GetElement(id);
	IGE.tooltip = str;
	AcceptElement(IGE);
}

float globalfontsize = 1;
void SetImGuiFontSize(float size) {
	globalfontsize = size;
}

void RenderText(ImGuiElement_ IGE) {
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4((float)IGE.color.r / 255, (float)IGE.color.g / 255, (float)IGE.color.b / 255, (float)IGE.color.a / 255));
	ImGui::TextUnformatted(StringToConstChar(IGE.text));
	ImGui::PopStyleColor();
}

void RenderError(ImGuiElement_ IGE) {
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,0,0,1));
	ImGui::TextUnformatted("!Error Element!");
	ImGui::PopStyleColor();
}

void RenderButton(ImGuiElement_ IGE, bool smallbutton, bool lb, bool rb) {
	if ((smallbutton? ImGui::SmallButton(StringToConstChar(IGE.text)) : (lb? ImGui::ArrowButton("", ImGuiDir_Left) : (rb? ImGui::ArrowButton("",ImGuiDir_Right) : ImGui::Button(StringToConstChar(IGE.text)))))) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func();
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, bool> Checkboxs;
void RenderCheckbox(ImGuiElement_ IGE) {
	if (Checkboxs.find(IGE.id) == Checkboxs.end()) {
		Checkboxs[IGE.id] = false;
	}
	if (ImGui::Checkbox(StringToConstChar(IGE.text), &Checkboxs[IGE.id])) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(Checkboxs[IGE.id]);
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, vector<string>> ComboItems;
unordered_map<string, int> ComboSelected;
void RenderCombo(ImGuiElement_ IGE) {
	if (ComboSelected.find(IGE.id) == ComboSelected.end()) {
		ComboSelected[IGE.id] = 0;
	}
	if (ComboItems.find(IGE.id) == ComboItems.end()) {
		ComboItems[IGE.id] = StringSplit(IGE.stringvalue, ',');
	}
	if (ImGui::BeginCombo(StringToConstChar(IGE.text), ComboItems[IGE.id][ComboSelected[IGE.id]].c_str())) {
		for (int i = 0; i < ComboItems[IGE.id].size(); i++) {
			bool isSelected = (ComboSelected[IGE.id] == i);
			if (ImGui::Selectable(ComboItems[IGE.id][i].c_str(), isSelected)) {
				int oldi = ComboSelected[IGE.id];
				string old = ComboItems[IGE.id][oldi];
				ComboSelected[IGE.id] = i;
				if (IGE.func != sol::nil && IGE.func.valid()) {
					try {
						IGE.func(ComboItems[IGE.id][i], i, old, oldi);
					}
					catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
						string what = e.what();
						PE(what, "IGLUA");
					}
				}
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

unordered_map<string, float> DragFloat;
void RenderDragFloat(ImGuiElement_ IGE) {
	if (DragFloat.find(IGE.id) == DragFloat.end()) {
		DragFloat[IGE.id] = 0;
	}
	if (ImGui::DragFloat(StringToConstChar(IGE.text), &DragFloat[IGE.id], IGE.floatvalue, IGE.min, IGE.max, StringToConstChar("%." + to_string(IGE.intvalue) + "f"))) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(DragFloat[IGE.id]);
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, float*> DragFloat2;
void RenderDragFloat2(ImGuiElement_ IGE) {
	if (DragFloat2.find(IGE.id) == DragFloat2.end()) {
		DragFloat2[IGE.id] = new float[2] {0, 0};
	}
	if (ImGui::DragFloat2(StringToConstChar(IGE.text), DragFloat2[IGE.id], IGE.floatvalue, IGE.min, IGE.max, StringToConstChar("%." + to_string(IGE.intvalue) + "f"))) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(DragFloat2[IGE.id][0], DragFloat2[IGE.id][1]);
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, float*> DragFloat3;
void RenderDragFloat3(ImGuiElement_ IGE) {
	if (DragFloat3.find(IGE.id) == DragFloat3.end()) {
		DragFloat3[IGE.id] = new float[3] {0, 0, 0};
	}
	if (ImGui::DragFloat3(StringToConstChar(IGE.text), DragFloat3[IGE.id], IGE.floatvalue, IGE.min, IGE.max, StringToConstChar("%." + to_string(IGE.intvalue) + "f"))) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(DragFloat3[IGE.id][0], DragFloat3[IGE.id][1], DragFloat3[IGE.id][2]);
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, float*> DragFloat4;
void RenderDragFloat4(ImGuiElement_ IGE) {
	if (DragFloat4.find(IGE.id) == DragFloat4.end()) {
		DragFloat4[IGE.id] = new float[4] {0, 0, 0, 0};
	}
	if (ImGui::DragFloat4(StringToConstChar(IGE.text), DragFloat4[IGE.id], IGE.floatvalue, IGE.min, IGE.max, StringToConstChar("%." + to_string(IGE.intvalue) + "f"))) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(DragFloat4[IGE.id][0], DragFloat4[IGE.id][1], DragFloat4[IGE.id][2], DragFloat4[IGE.id][3]);
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, float> SliderFloat;
void RenderSliderFloat(ImGuiElement_ IGE) {
	if (SliderFloat.find(IGE.id) == SliderFloat.end()) {
		SliderFloat[IGE.id] = 0;
	}
	if (ImGui::SliderFloat(StringToConstChar(IGE.text), &SliderFloat[IGE.id], IGE.min, IGE.max, StringToConstChar("%." + to_string(IGE.intvalue) + "f"))) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(SliderFloat[IGE.id]);
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, float*> SliderFloat2;
void RenderSliderFloat2(ImGuiElement_ IGE) {
	if (SliderFloat2.find(IGE.id) == SliderFloat2.end()) {
		SliderFloat2[IGE.id] = new float[2] {0, 0};
	}
	if (ImGui::SliderFloat2(StringToConstChar(IGE.text), SliderFloat2[IGE.id], IGE.min, IGE.max, StringToConstChar("%." + to_string(IGE.intvalue) + "f"))) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(SliderFloat2[IGE.id][0], SliderFloat2[IGE.id][1]);
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, float*> SliderFloat3;
void RenderSliderFloat3(ImGuiElement_ IGE) {
	if (SliderFloat3.find(IGE.id) == SliderFloat3.end()) {
		SliderFloat3[IGE.id] = new float[3] {0, 0, 0};
	}
	if (ImGui::SliderFloat3(StringToConstChar(IGE.text), SliderFloat3[IGE.id], IGE.min, IGE.max, StringToConstChar("%." + to_string(IGE.intvalue) + "f"))) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(SliderFloat3[IGE.id][0], SliderFloat3[IGE.id][1], SliderFloat3[IGE.id][2]);
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, float*> SliderFloat4;
void RenderSliderFloat4(ImGuiElement_ IGE) {
	if (SliderFloat4.find(IGE.id) == SliderFloat4.end()) {
		SliderFloat4[IGE.id] = new float[4] {0, 0, 0, 0};
	}
	if (ImGui::SliderFloat4(StringToConstChar(IGE.text), SliderFloat4[IGE.id], IGE.min, IGE.max, StringToConstChar("%." + to_string(IGE.intvalue) + "f"))) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(SliderFloat4[IGE.id][0], SliderFloat4[IGE.id][1], SliderFloat4[IGE.id][2], SliderFloat4[IGE.id][3]);
			}
			catch (const sol::error& e) { /*Получение ошибок из lua скриптов*/
				string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, string> InputText;
void RenderInputText(ImGuiElement_ IGE) {
	if (InputText.find(IGE.id) == InputText.end()) {
		InputText[IGE.id] = "";
	}

	char buffer[256];
	strncpy(buffer, InputText[IGE.id].c_str(), sizeof(buffer));
	if (ImGui::InputText(StringToConstChar(IGE.text), buffer, sizeof(buffer))) {
		InputText[IGE.id] = buffer;
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(InputText[IGE.id]);
			}
			catch (const sol::error& e) {
				std::string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, string> InputTextM;
void RenderInputTextM(ImGuiElement_ IGE) {
	if (InputTextM.find(IGE.id) == InputTextM.end()) {
		InputTextM[IGE.id] = "";
	}

	char buffer[1024];
	strncpy(buffer, InputTextM[IGE.id].c_str(), sizeof(buffer));
	if (ImGui::InputTextMultiline(StringToConstChar(IGE.text), buffer, sizeof(buffer), ImVec2(200, 100))) {
		InputTextM[IGE.id] = buffer;
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(InputTextM[IGE.id]);
			}
			catch (const sol::error& e) {
				std::string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, float*> EditRGB;
void RenderEditRGB(ImGuiElement_ IGE) {
	if (EditRGB.find(IGE.id) == EditRGB.end()) {
		EditRGB[IGE.id] = new float[3] {1, 1, 1};
	}

	if (ImGui::ColorEdit3(StringToConstChar(IGE.text), EditRGB[IGE.id])) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(l_Color((int)roundf(EditRGB[IGE.id][0] * 255), (int)roundf(EditRGB[IGE.id][1] * 255), (int)roundf(EditRGB[IGE.id][2] * 255)));
			}
			catch (const sol::error& e) {
				std::string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, float*> EditRGBA;
void RenderEditRGBA(ImGuiElement_ IGE) {
	if (EditRGBA.find(IGE.id) == EditRGBA.end()) {
		EditRGBA[IGE.id] = new float[4] {1, 1, 1, 1};
	}

	if (ImGui::ColorEdit4(StringToConstChar(IGE.text), EditRGBA[IGE.id])) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(l_Color((int)roundf(EditRGBA[IGE.id][0] * 255), (int)roundf(EditRGBA[IGE.id][1] * 255), (int)roundf(EditRGBA[IGE.id][2] * 255), (int)roundf(EditRGBA[IGE.id][3] * 255)));
			}
			catch (const sol::error& e) {
				std::string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, float*> EditRGBE;
void RenderEditRGBExtended(ImGuiElement_ IGE) {
	if (EditRGBE.find(IGE.id) == EditRGBE.end()) {
		EditRGBE[IGE.id] = new float[3] {1, 1, 1};
	}

	if (ImGui::ColorPicker3(StringToConstChar(IGE.text), EditRGBE[IGE.id])) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(l_Color((int)roundf(EditRGBE[IGE.id][0] * 255), (int)roundf(EditRGBE[IGE.id][1] * 255), (int)roundf(EditRGBE[IGE.id][2] * 255)));
			}
			catch (const sol::error& e) {
				std::string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

unordered_map<string, float*> EditRGBAE;
void RenderEditRGBAExtended(ImGuiElement_ IGE) {
	if (EditRGBAE.find(IGE.id) == EditRGBAE.end()) {
		EditRGBAE[IGE.id] = new float[4] {1, 1, 1, 1};
	}

	if (ImGui::ColorPicker4(StringToConstChar(IGE.text), EditRGBAE[IGE.id])) {
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(l_Color((int)roundf(EditRGBAE[IGE.id][0] * 255), (int)roundf(EditRGBAE[IGE.id][1] * 255), (int)roundf(EditRGBAE[IGE.id][2] * 255), (int)roundf(EditRGBAE[IGE.id][3] * 255)));
			}
			catch (const sol::error& e) {
				std::string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
}

float color[3] = { 1,0,0 };
void RenderWindow(ImGuiWindow_ IGW) {
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::Begin(StringToConstChar(IGW.title),NULL, ImGuiWindowFlags_NoSavedSettings);
	for (const auto& pair : zpos) {
		ImGuiElement_ el = Elements[pair.second];
		if (el.windowid == IGW.id) {
			string type = el.type;
			ImGuiStyle& style = ImGui::GetStyle();
			if (el.sameline) {
				ImGui::SameLine(0, style.ItemInnerSpacing.x);
			}
			if (el.hasbullet) {
				ImGui::Bullet();
			}
			if (type == "text") {
				RenderText(el);
			}
			else if (type == "button" || type == "small button" || type == "left arrow button" || type == "right arrow button") {
				RenderButton(el, type == "small button", type == "left arrow button", type == "right arrow button");
			}
			else if (type == "checkbox") {
				RenderCheckbox(el);
			}
			else if (type == "list") {
				RenderCombo(el);
			}
			else if (type == "drag number") {
				RenderDragFloat(el);
			}
			else if (type == "drag number 2") {
				RenderDragFloat2(el);
			}
			else if (type == "drag number 3") {
				RenderDragFloat3(el);
			}
			else if (type == "drag number 4") {
				RenderDragFloat4(el);
			}
			else if (type == "slider number") {
				RenderSliderFloat(el);
			}
			else if (type == "slider number 2") {
				RenderSliderFloat2(el);
			}
			else if (type == "slider number 3") {
				RenderSliderFloat3(el);
			}
			else if (type == "slider number 4") {
				RenderSliderFloat4(el);
			}
			else if (type == "input text") {
				RenderInputText(el);
			}
			else if (type == "input text multiline") {
				RenderInputTextM(el);
			}
			else if (type == "edit rgb") {
				RenderEditRGB(el);
			}
			else if (type == "edit rgba") {
				RenderEditRGBA(el);
			}
			else if (type == "edit rgb extended") {
				RenderEditRGBExtended(el);
			}
			else if (type == "edit rgba extended") {
				RenderEditRGBAExtended(el);
			}
			else if (type == "test") {
				
			}
			else if (type == "error") {
				RenderError(el);
			}
			else {
				PE("This type ['"+el.type+"'] of ImGui element does not exist! RenderWindow()","E0042");
				el.type = "error";
				AcceptElement(el);
			}

			if (el.tooltip != "") {
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip(StringToConstChar(el.tooltip));
				}
			}
		}
	}
	ImGui::End();
}

void ImGuiStyle_() {
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.06f, 0.06f, 0.94f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.25f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.04f, 0.04f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.50f, 0.43f, 0.43f, 0.50f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.48f, 0.16f, 0.16f, 0.54f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.11f, 0.11f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.88f, 0.24f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.06f, 0.06f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.98f, 0.26f, 0.26f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.43f, 0.43f, 0.50f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.10f, 0.10f, 0.78f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.10f, 0.10f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.26f, 0.26f, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.18f, 0.18f, 0.86f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.07f, 0.07f, 0.97f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.26f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void ImGuiRender(string windowid) {
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = globalfontsize;

	for (const auto& pair : Windows) {
		if (pair.second.active) {
			RenderWindow(pair.second);
		}
	}
}