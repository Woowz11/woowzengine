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
#include "WConst.h"

using namespace std;

vector<string> ElementTypes{"text","button","checkbox"};

unordered_map<string, ImGuiWindow_> Windows;
unordered_map<string, ImGuiElement_> Elements;

int imguielementid = 0;
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

unordered_map<string, bool> Checkboxs;
unordered_map<string, vector<string>> ComboItems;
unordered_map<string, int> ComboSelected;
unordered_map<string, float> DragFloat;
unordered_map<string, float*> DragFloat2;
unordered_map<string, float*> DragFloat3;
unordered_map<string, float*> DragFloat4;
unordered_map<string, float> SliderFloat;
unordered_map<string, float*> SliderFloat2;
unordered_map<string, float*> SliderFloat3;
unordered_map<string, float*> SliderFloat4;
unordered_map<string, string> InputText;
unordered_map<string, string*> InputText2;
unordered_map<string, string> InputTextM;
unordered_map<string, float*> EditRGB;
unordered_map<string, float*> EditRGBA;
unordered_map<string, float*> EditRGBE;
unordered_map<string, float*> EditRGBAE;

void CreateImGuiElement(string id, string window, string type) {
	if (Elements.find(id) == Elements.end()) {
		zindexs++;
		ImGuiElement_ IGE = ImGuiElement_(id, window, type, zindexs);
		Elements[id] = IGE;
		zpos[zindexs] = id;
		if (IGE.type == "checkbox") {
			if (Checkboxs.find(IGE.id) == Checkboxs.end()) {
				Checkboxs[IGE.id] = false;
			}
		}else if(IGE.type == "list") {
			if (ComboSelected.find(IGE.id) == ComboSelected.end()) {
				ComboSelected[IGE.id] = 0;
			}
			if (ComboItems.find(IGE.id) == ComboItems.end()) {
				ComboItems[IGE.id] = StringSplit(IGE.stringvalue, ',');
			}
		}
		else if (IGE.type == "drag number") {
			if (DragFloat.find(IGE.id) == DragFloat.end()) {
				DragFloat[IGE.id] = 0;
			}
		}
		else if (IGE.type == "drag number 2") {
			if (DragFloat2.find(IGE.id) == DragFloat2.end()) {
				DragFloat2[IGE.id] = new float[2] {0, 0};
			}
		}
		else if (IGE.type == "drag number 3") {
			if (DragFloat3.find(IGE.id) == DragFloat3.end()) {
				DragFloat3[IGE.id] = new float[3] {0, 0, 0};
			}
		}
		else if (IGE.type == "drag number 4") {
			if (DragFloat4.find(IGE.id) == DragFloat4.end()) {
				DragFloat4[IGE.id] = new float[4] {0, 0, 0, 0};
			}
		}
		else if (IGE.type == "slider number") {
			if (SliderFloat.find(IGE.id) == SliderFloat.end()) {
				SliderFloat[IGE.id] = 0;
			}
		}
		else if (IGE.type == "slider number 2") {
			if (SliderFloat2.find(IGE.id) == SliderFloat2.end()) {
				SliderFloat2[IGE.id] = new float[2] {0, 0};
			}
		}
		else if (IGE.type == "slider number 3") {
			if (SliderFloat3.find(IGE.id) == SliderFloat3.end()) {
				SliderFloat3[IGE.id] = new float[3] {0, 0, 0};
			}
		}
		else if (IGE.type == "slider number 4") {
			if (SliderFloat4.find(IGE.id) == SliderFloat4.end()) {
				SliderFloat4[IGE.id] = new float[4] {0, 0, 0, 0};
			}
		}
		else if (IGE.type == "input text") {
			if (InputText.find(IGE.id) == InputText.end()) {
				InputText[IGE.id] = "";
			}
		}
		else if (IGE.type == "input text 2") {
			if (InputText2.find(IGE.id) == InputText2.end()) {
				InputText2[IGE.id] = new string[2]{"",""};
			}
		}
		else if (IGE.type == "input text multiline") {
			if (InputTextM.find(IGE.id) == InputTextM.end()) {
				InputTextM[IGE.id] = "";
			}
		}
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
	if (IGE.type == "list") {
		ComboSelected[IGE.id] = 0;
		ComboItems[IGE.id] = StringSplit(IGE.stringvalue, ',');
	}
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

void RenderCheckbox(ImGuiElement_ IGE) {
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

void RenderCombo(ImGuiElement_ IGE) {
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

void RenderDragFloat(ImGuiElement_ IGE) {
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

void RenderDragFloat2(ImGuiElement_ IGE) {
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

void RenderDragFloat3(ImGuiElement_ IGE) {
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

void RenderDragFloat4(ImGuiElement_ IGE) {
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

void RenderSliderFloat(ImGuiElement_ IGE) {
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

void RenderSliderFloat2(ImGuiElement_ IGE) {
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

void RenderSliderFloat3(ImGuiElement_ IGE) {
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

void RenderSliderFloat4(ImGuiElement_ IGE) {
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

void RenderInputText(ImGuiElement_ IGE) {
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

void RenderInputText2(ImGuiElement_ IGE) {
	float width = ImGui::GetWindowSize().x / 3.1;
	char buffer[256];
	strncpy(buffer, InputText2[IGE.id][0].c_str(), sizeof(buffer));
	ImGui::SetNextItemWidth(width);
	if (ImGui::InputText("", buffer, sizeof(buffer))) {
		InputText2[IGE.id][0] = buffer;
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(InputText2[IGE.id][0], InputText2[IGE.id][1], true);
			}
			catch (const sol::error& e) {
				std::string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
	ImGui::SameLine();
	ImGui::SetNextItemWidth(width);
	ImGui::PushID(imguielementid);
	imguielementid++;
	char buffer2[256];
	strncpy(buffer2, InputText2[IGE.id][1].c_str(), sizeof(buffer2));
	if (ImGui::InputText(StringToConstChar(IGE.text), buffer2, sizeof(buffer2))) {
		InputText2[IGE.id][1] = buffer2;
		if (IGE.func != sol::nil && IGE.func.valid()) {
			try {
				IGE.func(InputText2[IGE.id][0], InputText2[IGE.id][1], false);
			}
			catch (const sol::error& e) {
				std::string what = e.what();
				PE(what, "IGLUA");
			}
		}
	}
	ImGui::PopID();
}

void RenderInputTextM(ImGuiElement_ IGE) {
	char buffer[1024];
	strncpy(buffer, InputTextM[IGE.id].c_str(), sizeof(buffer));
	if (ImGui::InputTextMultiline(StringToConstChar(IGE.text), buffer, sizeof(buffer), ImVec2(ImGui::GetWindowSize().x / 1.55 , 100))) {
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

void SetImGuiElementValue(string id, vector<variant<string, double, bool, l_Color>> val1, vector<variant<string, double, bool, l_Color>> val2, vector<variant<string, double, bool, l_Color>> val3, vector<variant<string, double, bool, l_Color>> val4) {
	ImGuiElement_ el = GetElement(id);
	if (el.id != ErrorElement) {
		string str = get<string>(val1[0]);
		string str2 = get<string>(val2[0]);
		string str3 = get<string>(val3[0]);
		string str4 = get<string>(val4[0]);
		double num = get<double>(val1[1]);
		double num2 = get<double>(val2[1]);
		double num3 = get<double>(val3[1]);
		double num4 = get<double>(val4[1]);
		bool bol = get<bool>(val1[2]);
		bool bol2 = get<bool>(val2[2]);
		bool bol3 = get<bool>(val3[2]);
		bool bol4 = get<bool>(val4[2]);
		l_Color col = get<l_Color>(val1[3]);
		l_Color col2 = get<l_Color>(val2[3]);
		l_Color col3 = get<l_Color>(val3[3]);
		l_Color col4 = get<l_Color>(val4[3]);

		if (el.type == "checkbox") {
			Checkboxs[el.id] = bol;
		}
		else if (el.type == "list") {
			auto it = find(ComboItems[el.id].begin(), ComboItems[el.id].end(), str);
			if (it != ComboItems[el.id].end()) {
				ComboSelected[el.id] = distance(ComboItems[el.id].begin(), it);
			}
			else {
				PE("Not found item ['"+str+"'] in list! SetImGuiElementValue('"+id+"','"+str+"')","L0048");
			}
		}
		else if (el.type == "drag number") {
			DragFloat[el.id] = num;
		}
		else if (el.type == "drag number 2") {
			DragFloat2[el.id][0] = num;
			DragFloat2[el.id][1] = num2;
		}
		else if (el.type == "drag number 3") {
			DragFloat3[el.id][0] = num;
			DragFloat3[el.id][1] = num2;
			DragFloat3[el.id][2] = num3;
		}
		else if (el.type == "drag number 4") {
			DragFloat4[el.id][0] = num;
			DragFloat4[el.id][1] = num2;
			DragFloat4[el.id][2] = num3;
			DragFloat4[el.id][3] = num4;
		}
		else if (el.type == "slider number") {
			SliderFloat[el.id] = num;
		}
		else if (el.type == "slider number 2") {
			SliderFloat2[el.id][0] = num;
			SliderFloat2[el.id][1] = num2;
		}
		else if (el.type == "slider number 3") {
			SliderFloat3[el.id][0] = num;
			SliderFloat3[el.id][1] = num2;
			SliderFloat3[el.id][2] = num3;
		}
		else if (el.type == "slider number 4") {
			SliderFloat4[el.id][0] = num;
			SliderFloat4[el.id][1] = num2;
			SliderFloat4[el.id][2] = num3;
			SliderFloat4[el.id][3] = num4;
		}
		else if (el.type == "input text") {
			InputText[el.id] = str;
		}
		else if (el.type == "input text 2") {
			InputText2[el.id][0] = str;
			InputText2[el.id][1] = str2;
		}
		else if (el.type == "input text multiline") {
			InputTextM[el.id] = str;
		}
		else if (el.type == "edit rgb") {
			EditRGB[el.id] = new float[3] {(float)col.r / 255, (float)col.g / 255, (float)col.b / 255};
		}
		else if (el.type == "edit rgb extended") {
			EditRGBE[el.id] = new float[3] {(float)col.r / 255, (float)col.g / 255, (float)col.b / 255};
		}
		else if (el.type == "edit rgba") {
			EditRGBA[el.id] = new float[4] {(float)col.r / 255, (float)col.g / 255, (float)col.b / 255, (float)col.a / 255};
		}
		else if (el.type == "edit rgba extended") {
			EditRGBAE[el.id] = new float[4] {(float)col.r / 255, (float)col.g / 255, (float)col.b / 255, (float)col.a / 255};
		}
		else {
			PE("ImGui Element ['"+id+"'] type ['"+el.type+"'] don't accept values! SetImGuiElementValue('"+id+"','"+str+"')","L0016");
		}
	}
}

variant<string, vector<string>, double,vector<double>,bool,l_Color> GetImGuiElementValue(string id) {
	ImGuiElement_ el = GetElement(id);

	if (el.id != ErrorElement) {
		if (el.type == "checkbox") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>((bool)Checkboxs[el.id]);
		}
		else if (el.type == "list") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>((string)ComboItems[el.id][ComboSelected[el.id]]);
		}
		else if (el.type == "drag number") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>((double)DragFloat[el.id]);
		}
		else if (el.type == "drag number 2") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>(vector<double>{ DragFloat2[el.id][0], DragFloat2[el.id][1]});
		}
		else if (el.type == "drag number 3") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>(vector<double>{ DragFloat3[el.id][0], DragFloat3[el.id][1], DragFloat3[el.id][2]});
		}
		else if (el.type == "drag number 4") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>(vector<double>{ DragFloat4[el.id][0], DragFloat4[el.id][1], DragFloat4[el.id][2], DragFloat4[el.id][3]});
		}
		else if (el.type == "slider number") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>((double)SliderFloat[el.id]);
		}
		else if (el.type == "slider number 2") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>(vector<double>{ SliderFloat2[el.id][0], SliderFloat2[el.id][1]});
		}
		else if (el.type == "slider number 3") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>(vector<double>{ SliderFloat3[el.id][0], SliderFloat3[el.id][1], SliderFloat3[el.id][2]});
		}
		else if (el.type == "slider number 4") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>(vector<double>{ SliderFloat4[el.id][0], SliderFloat4[el.id][1], SliderFloat4[el.id][2], SliderFloat4[el.id][3]});
		}
		else if (el.type == "input text") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>((string)InputText[el.id]);
		}
		else if (el.type == "input text 2") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>(vector<string>{ InputText2[el.id][0], InputText2[el.id][1] });
		}
		else if (el.type == "input text multiline") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>((string)InputTextM[el.id]);
		}
		else if (el.type == "edit rgb") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>(l_Color((int)(EditRGB[el.id][0] * 255), (int)(EditRGB[el.id][1] * 255), (int)(EditRGB[el.id][2] * 255)));
		}
		else if (el.type == "edit rgb extended") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>(l_Color((int)(EditRGBE[el.id][0] * 255), (int)(EditRGBE[el.id][1] * 255), (int)(EditRGBE[el.id][2] * 255)));
		}
		else if (el.type == "edit rgba") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>(l_Color((int)(EditRGBA[el.id][0] * 255), (int)(EditRGBA[el.id][1] * 255), (int)(EditRGBA[el.id][2] * 255), (int)(EditRGBA[el.id][3] * 255)));
		}
		else if (el.type == "edit rgba extended") {
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>(l_Color((int)(EditRGBAE[el.id][0] * 255), (int)(EditRGBAE[el.id][1] * 255), (int)(EditRGBAE[el.id][2] * 255), (int)(EditRGBAE[el.id][3] * 255)));
		}
		else {
			PE("ImGui Element ['" + id + "'] type [" + el.type + "] don't accept values! GetImGuiElementValue('" + id + "')", "L0045");
			return variant<string, vector<string>, double, vector<double>, bool, l_Color>("");
		}
	}
	return variant<string, vector<string>, double, vector<double>, bool, l_Color>("Epic fail... error");
}

void RenderWindow(ImGuiWindow_ IGW) {
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::Begin(StringToConstChar(IGW.title),NULL, ImGuiWindowFlags_NoSavedSettings);
	for (const auto& pair : zpos) {
		ImGuiElement_ el = Elements[pair.second];
		if (el.windowid == IGW.id) {
			string type = el.type;
			ImGuiStyle& style = ImGui::GetStyle();

			ImGui::PushID(imguielementid);
			imguielementid++;
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
			else if (type == "input text 2") {
				RenderInputText2(el);
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

			ImGui::PopID();
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

	imguielementid = 0;
	for (const auto& pair : Windows) {
		if (pair.second.active) {
			RenderWindow(pair.second);
		}
	}
}