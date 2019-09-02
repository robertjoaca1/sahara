#pragma once
#pragma once

#include <string>
#include "../singleton.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/directx9/imgui_impl_dx9.h"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/input.hpp"
#include "../options.hpp"
#include "../ui.hpp"
#include <d3dx9tex.h>
#pragma comment(lib,"d3dx9.lib")

struct IDirect3DDevice9;

class Menu
	: public Singleton<Menu>
{
public:
	void Initialize();
	void Shutdown();

	void OnDeviceLost();
	void OnDeviceReset();

	void RenderMenu();

	void Show();
	void Hide();
	void Toggle();

	bool IsVisible() const { return Visible; }

private:
	void CreateStyle();

	ImGuiStyle	Style;
	bool		Visible;
	bool		LegitAimbot;
	bool		LegitTriggerbot;
	bool		LegitBacktrack;
	bool		VisualsChams;
	bool		VisualsGlow;
};