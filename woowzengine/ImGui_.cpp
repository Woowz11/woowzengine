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

void CreateImGuiWindow(string id, string title) {
	ImGuiWindow_ IGW = ImGuiWindow_(id);
	IGW.title = title;
	Windows[id] = IGW;
}

void CreateImGuiElement(string id, string window, string type) {
	ImGuiElement_ IGE = ImGuiElement_(id, window, Lowercase(type));
	Elements[id] = IGE;
}

void AcceptElement(ImGuiElement_ IGE) {
	Elements[IGE.id] = IGE;
}

ImGuiElement_ GetElement(string id) {
	return Elements[id];
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

void RenderText(ImGuiElement_ IGE) {
	ImGui::TextColored(ImVec4((float)IGE.color.r / 255, (float)IGE.color.g / 255, (float)IGE.color.b / 255, (float)IGE.color.a / 255), StringToConstChar(IGE.text));
}

void RenderButton(ImGuiElement_ IGE) {
	if (ImGui::Button(StringToConstChar(IGE.text))) {
		IGE.func();
	}
}

void RenderWindow(ImGuiWindow_ IGW) {
	ImGui::Begin(StringToConstChar(IGW.title));
	for (const auto& pair : Elements) {
		if (pair.second.windowid == IGW.id) {
			string type = pair.second.type;
			if (type == "text") {
				RenderText(pair.second);
			}
			else if (type == "button") {
				RenderButton(pair.second);
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
	for (const auto& pair : Windows) {
		RenderWindow(pair.second);
	}
	//ImGui::ShowDemoWindow();
}