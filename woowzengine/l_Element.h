#include <list>
#include <lua/lua.hpp>
#include <sol/sol.hpp>
#include "RenderElement.h"
#include "l_Vertex.h"
#include "Vertex.h"
#include "Base.h"

#pragma once

class l_Element {
public:
	l_Element() {}
	l_Element(string id) : ID(id) {}

	string ID = "new element";
	string RenderType = "";
	std::list<l_Vertex> Vertexs = {};
	int ZIndex = 0;
	bool DontScale = false;

	void AddVertex(l_Vertex v) {
		Vertexs.push_back(v);
	}

	void ClearVertexs() {
		Vertexs = {};
	}
	
	void SetType(string type, std::list<l_Vertex> vert) {
		bool next = true;
		bool error = false;
		int size = vert.size();
		if (type == "triangle") {
			if (size != 3) {
				PE("Element ["+ID+"] of type triangle must have 3 vertexes! Has ["+to_string(size) + "]", "L0016");
				error = true;
			}
		}
		else {
			next = false;
			PE("Type not found for element ["+ID+"]! SetType('"+type+"',List<Vertex>("+to_string(size) + "))", "L0015");
		}
		if (next&&!error) {
			RenderType = type;
			Vertexs = vert;
		}
	}

	void SetTypeLua(string type, sol::table vert) {
		std::list<l_Vertex> vert_ = {};

		for (auto const& pair : vert) {
			vert_.push_back(pair.second.as<l_Vertex>());
		}

		SetType(type, vert_);
	}

	RenderElement ToCPP() {
		RenderElement re = RenderElement(ID);
		re.type = RenderType;
		re.ZIndex = ZIndex;
		re.DontScale = DontScale;
		std::list<Vertex> Vertexs_ = {};
		for (l_Vertex v : Vertexs) {
			Vertexs_.push_back(v.ToCPP());
		}
		re.Vertexs = Vertexs_;
		return re;
	}
};