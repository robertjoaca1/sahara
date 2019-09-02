#include "menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <chrono>
#include "../config.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS

void Menu::Initialize()
{
	/*D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9
		, &SaharaLogo, sizeof(SaharaLogo),
		101, 101, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &LogoImage);*/
	/*D3DXCreateTextureFromFileInMemory(g_D3DDevice9
		, &SaharaLogo, sizeof(SaharaLogo), &LogoImage);*/
	Visible = false;
	LegitAimbot = false;
	LegitTriggerbot = false;
	LegitBacktrack = false;
	VisualsChams = false;
	VisualsGlow = false;


	ImGui::CreateContext();

	ImGui_ImplDX9_Init(InputSys::Get().GetMainWindow(), g_D3DDevice9);

	CreateStyle();
}

void Menu::Shutdown()
{
	ImGui_ImplDX9_Shutdown();
}

void Menu::OnDeviceLost()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}
void Menu::RenderMenu()
{
	if (!Visible)
		return;

	ImGui_ImplDX9_NewFrame();

	ImGui::GetIO().MouseDrawCursor = Visible;
	ImGui::SetNextWindowPosCenter(ImGuiSetCond_Once);
	//ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2{ 250, 300 }, ImGuiSetCond_Once);
	if (ImGui::Begin("Sahara CS:GO", &Visible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		if (ImGui::CollapsingHeader("Ragebot##rageheader"))
		{
			ImGui::Checkbox("Enabled##rageaimbot", &Variables.RageAimbotEnabled);
			ImGui::SliderInt("Hitchance", &Variables.RageAimbotHitchance, 0, 100);
			ImGui::SliderInt("Min Dmg", &Variables.RageAimbotMinDmg, 0, 100);
			ImGui::Checkbox("Antiaim##aaenable", &Variables.RageAntiaimEnabled);
		}
		if (ImGui::CollapsingHeader("Legitbot##legitheader"))
		{
			ImGui::Checkbox("Aimbot##legitaim", &Variables.LegitAimbotEnabled);
			if (Variables.LegitAimbotEnabled)
			{
				ImGui::Combo("Type##legitaim", &Variables.LegitAimbotType, LegitAimbotType, ARRAYSIZE(LegitAimbotType));
				ImGui::Combo("Hitbox##legitaim", &Variables.LegitAimbotHitbox, LegitAimbotHitbox, ARRAYSIZE(LegitAimbotHitbox));
				ImGui::SliderFloat("FOV##legitaim", &Variables.LegitAimbotFov, 0.f, 30.f, "%.1f");
				if (Variables.LegitAimbotType == 1)
					ImGui::SliderInt("Smooth##legitaim", &Variables.LegitAimbotSmooth, 0, 100);
				ImGui::SliderInt("Rcs##legitaim", &Variables.LegitAimbotRcs, 0, 100);
			}
			ImGui::Checkbox("Backtrack##legit", &Variables.LegitBacktrackEnabled);
			if (Variables.LegitBacktrackEnabled)
				ImGui::SliderInt("Time(ms)##backtracktime", &Variables.LegitBacktrackDuration, 1, 200);
		}
		if (ImGui::CollapsingHeader("Visuals##visualsheader"))
		{
			ImGui::Checkbox("Box", &Variables.VisualsBox);
			ImGui::Checkbox("Health", &Variables.VisualsHealth);
			ImGui::Checkbox("Name", &Variables.VisualsName);
			ImGui::Checkbox("Weapon", &Variables.VisualsWeapon);
			if (ImGui::CollapsingHeader("Chams##visualsheader"))
			{
				ImGui::Checkbox("Chams##chamsenabled", &Variables.VisualsChamsEnabled);
				if (Variables.VisualsChamsEnabled)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##chamscolor", Variables.VisualsChamsColor, ImGuiColorEditFlags_NoInputs);
					ImGui::Combo("Material##chams", &Variables.VisualsChamsMaterial, ChamsMaterials, ARRAYSIZE(ChamsMaterials));
					ImGui::SliderInt("Alpha##chamsalpha", &Variables.VisualsChamsAlpha, 0, 255);
					ImGui::Checkbox("Ignore-Z##chamsingorezenabled", &Variables.VisualsChamsIgnoreZ);
					ImGui::SameLine();
					ImGui::ColorEdit3("##chamscolorignorez", Variables.VisualsChamsColorIgnoreZ, ImGuiColorEditFlags_NoInputs);
					if (Variables.LegitBacktrackEnabled || Variables.RageAimbotEnabled)
					{
						ImGui::Text("Backtrack");
						ImGui::SameLine();
						ImGui::ColorEdit3("##chamscolorbacktrack", Variables.VisualsChamsBacktrackColor, ImGuiColorEditFlags_NoInputs);
						ImGui::Combo("Backtrack", &Variables.VisualsChamsBacktrack, ChamsBacktrack, ARRAYSIZE(ChamsBacktrack));
						ImGui::Combo("Material##backtrack", &Variables.VisualsChamsBacktrackMaterial, ChamsMaterials, ARRAYSIZE(ChamsMaterials));
						ImGui::SliderInt("Alpha##backtrackchamsalpha", &Variables.VisualsChamsBacktrackAlpha, 0, 255);
					}
				}
			}
			if (ImGui::CollapsingHeader("Glow##visualsheader"))
			{
				ImGui::Checkbox("Enemy Glow##glowenabled", &Variables.VisualsGlowEnabled);
				if (Variables.VisualsGlowEnabled)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##glowcolor", Variables.VisualsGlowColor, ImGuiColorEditFlags_NoInputs);
					ImGui::Combo("Glowstyle##glow", &Variables.VisualsGlowGlowstyle, GlowStyles, ARRAYSIZE(GlowStyles));
					ImGui::SliderInt("Alpha##glowalpha", &Variables.VisualsGlowAlpha, 0, 255);
				}
			}
		}
		if (ImGui::CollapsingHeader("Misc##mischeader"))
		{
			ImGui::Combo("Config", &Variables.ConfigFile, ConfigFiles, ARRAYSIZE(ConfigFiles));
			if (ImGui::Button("Save", ImVec2(ImGui::GetCurrentWindow()->DC.ItemWidth, 18)))
				SaveConfig();
			if (ImGui::Button("Load", ImVec2(ImGui::GetCurrentWindow()->DC.ItemWidth, 18)))
				LoadConfig();
			ImGui::SliderInt("Fakelag", &Variables.MiscFakelagChoke, 0, 15);
			ImGui::Checkbox("No Scope Border", &Variables.VisualsNoScope);
		}
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
/*//ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiSetCond_Once);
ImGui::SetNextWindowPosCenter(ImGuiSetCond_Once);
ImGui::SetNextWindowSize(ImVec2{ 580, 380 }, ImGuiSetCond_Once);
if (ImGui::Begin("Sahara CS:GO", &Visible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
{
	ImGui::PushItemWidth(130.f);
	Style.ItemSpacing = ImVec2(0, 0);
	if (ImGui::BeginChild("##tabchild", ImVec2(120, -1), true))
	{
		//ImGui::Image((void*)LogoImage, ImVec2(131, 131));
		//for (int i = 0; i < 3; i++)
		//	ImGui::Spacing();
		for (int i = 0; i < ARRAYSIZE(MenuTabs); i++)
		{
			if (ImGui::Button2(MenuTabs[i], ImVec2(-1, 14), MenuTab == i))
				MenuTab = i;
		}
		ImGui::EndChild();
	}
	Style.ItemSpacing = ImVec2(8, 4);
	ImGui::SameLine();
	if (ImGui::BeginChild("##featurechild", ImVec2(-1, -1), true))
	{
		switch (MenuTab)
		{
		case 0:
		{
			ImGui::Columns(2, "###aimbotcolumn", true);

			ImGui::Checkbox("Aimbot##legitaim", &Variables.LegitAimbotEnabled);
			if (Variables.LegitAimbotEnabled)
			{
				ImGui::Combo("Type##legitaim", &Variables.LegitAimbotType, LegitAimbotType, ARRAYSIZE(LegitAimbotType));
				ImGui::Combo("Hitbox##legitaim", &Variables.LegitAimbotHitbox, LegitAimbotHitbox, ARRAYSIZE(LegitAimbotHitbox));
				ImGui::SliderFloat("FOV##legitaim", &Variables.LegitAimbotFov, 0.f, 30.f, "%.1f");
				if (Variables.LegitAimbotType == 1)
					ImGui::SliderInt("Smooth##legitaim", &Variables.LegitAimbotSmooth, 0, 100);
				ImGui::SliderInt("Rcs##legitaim", &Variables.LegitAimbotRcs, 0, 100);
			}

			ImGui::NextColumn();

			ImGui::Checkbox("Backtrack##legit", &Variables.LegitBacktrackEnabled);
			if (Variables.LegitBacktrackEnabled)
				ImGui::SliderInt("Time(ms)##backtracktime", &Variables.LegitBacktrackDuration, 1, 200);
		}
			break;
		case 1:
			break;
		case 2:
		{
			for (int i = 0; i < ARRAYSIZE(VisualsTabs); i++)
			{
				if (ImGui::Button2(VisualsTabs[i], ImVec2(422 / ARRAYSIZE(VisualsTabs), 14), VisualsTab == i))
					VisualsTab = i;
				if (i != 2) ImGui::SameLine(0, 0);
			}
			switch (VisualsTab)
			{
			case 0:
				ImGui::Checkbox("Box", &Variables.VisualsBox);
				ImGui::Checkbox("Health", &Variables.VisualsHealth);
				break;
			case 1:
				ImGui::Columns(2, "###chamscolumn", true);

				ImGui::Checkbox("Chams##chamsenabled", &Variables.VisualsChamsEnabled);
				if (Variables.VisualsChamsEnabled)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##chamscolor", Variables.VisualsChamsColor, ImGuiColorEditFlags_NoInputs);
					ImGui::Combo("Material##chams", &Variables.VisualsChamsMaterial, ChamsMaterials, ARRAYSIZE(ChamsMaterials));
					ImGui::SliderInt("Alpha##chamsalpha", &Variables.VisualsChamsAlpha, 0, 255);
					ImGui::Checkbox("Ignore-Z##chamsingorezenabled", &Variables.VisualsChamsIgnoreZ);
					ImGui::SameLine();
					ImGui::ColorEdit3("##chamscolorignorez", Variables.VisualsChamsColorIgnoreZ, ImGuiColorEditFlags_NoInputs);

				}
				ImGui::NextColumn();
				if (Variables.VisualsChamsEnabled && Variables.LegitBacktrackEnabled);
				{
					ImGui::Text("Backtrack");
					ImGui::SameLine();
					ImGui::ColorEdit3("##chamscolorbacktrack", Variables.VisualsChamsBacktrackColor, ImGuiColorEditFlags_NoInputs);
					ImGui::Combo("Backtrack", &Variables.VisualsChamsBacktrack, ChamsBacktrack, ARRAYSIZE(ChamsBacktrack));
					ImGui::Combo("Material##backtrack", &Variables.VisualsChamsBacktrackMaterial, ChamsMaterials, ARRAYSIZE(ChamsMaterials));
					ImGui::SliderInt("Backtrack Alpha##chamsalpha", &Variables.VisualsChamsBacktrackAlpha, 0, 255);
				}

				break;
			case 2:
				ImGui::Checkbox("Enemy Glow##glowenabled", &Variables.VisualsGlowEnabled);
				if (Variables.VisualsGlowEnabled)
				{
					ImGui::SameLine();
					ImGui::ColorEdit3("##glowcolor", Variables.VisualsGlowColor, ImGuiColorEditFlags_NoInputs);
					ImGui::Combo("Glowstyle##glow", &Variables.VisualsGlowGlowstyle, GlowStyles, ARRAYSIZE(GlowStyles));
					ImGui::SliderInt("Alpha##glowalpha", &Variables.VisualsGlowAlpha, 0, 255);
				}
				break;
			}
		}
			break;
		}
		ImGui::EndChild();
	}
	ImGui::SameLine();
	if (ImGui::BeginChild("##featurechild", ImVec2(-1, -1), true))
	{
		switch (MenuTab)
		{
		case 0: // legit
			ImGui::Columns(2, "###aimbotcolumn", true);
			ImGui::PushItemWidth(130);

			ImGui::Checkbox("Aimbot##legitaim", &Variables.LegitAimbotEnabled);
			if (Variables.LegitAimbotEnabled)
			{
				ImGui::Combo("Type##legitaim", &Variables.LegitAimbotType, LegitAimbotType, ARRAYSIZE(LegitAimbotType));
				ImGui::Combo("Hitbox##legitaim", &Variables.LegitAimbotHitbox, LegitAimbotHitbox, ARRAYSIZE(LegitAimbotHitbox));
				ImGui::SliderFloat("FOV##legitaim", &Variables.LegitAimbotFov, 0.f, 10.f, "%.1f");
				if (Variables.LegitAimbotType == 1)
					ImGui::SliderInt("Smooth##legitaim", &Variables.LegitAimbotSmooth, 0, 100);
			}

			ImGui::PopItemWidth();
			ImGui::NextColumn();
			ImGui::PushItemWidth(130);

			ImGui::Checkbox("Backtrack##legit", &Variables.LegitBacktrackEnabled);
			if (Variables.LegitBacktrackEnabled)
				ImGui::SliderInt("Backtrack Duration##legit", &Variables.LegitBacktrackDuration, 1, 200);

			ImGui::PopItemWidth();
			break;
		case 1: // rage
			ImGui::PushItemWidth(130);

			ImGui::PopItemWidth();
			break;
		case 2: // visuals
			ImGui::Columns(2, "###visualtabcolumn", true);

			ImGui::PushItemWidth(130);
			ImGui::Checkbox("Chams##chamsenabled", &Variables.VisualsChamsEnabled);
			if (Variables.VisualsChamsEnabled)
			{
				ImGui::SameLine();
				ImGui::ColorEdit3("##chamscolor", Variables.VisualsChamsColor, ImGuiColorEditFlags_NoInputs);
				ImGui::Combo("Material##chams", &Variables.VisualsChamsMaterial, ChamsMaterials, ARRAYSIZE(ChamsMaterials));
				ImGui::SliderInt("Alpha##chamsalpha", &Variables.VisualsChamsAlpha, 0, 255);
				ImGui::Checkbox("Ignore-Z##chamsingorezenabled", &Variables.VisualsChamsIgnoreZ);
				ImGui::SameLine();
				ImGui::ColorEdit3("##chamscolorignorez", Variables.VisualsChamsColorIgnoreZ, ImGuiColorEditFlags_NoInputs);
				if (Variables.LegitBacktrackEnabled)
				{
					ImGui::Combo("Backtrack", &Variables.VisualsChamsBacktrack, ChamsBacktrack, ARRAYSIZE(ChamsBacktrack));
					ImGui::SameLine();
					ImGui::ColorEdit3("##chamscolorbacktrack", Variables.VisualsChamsBacktrackColor, ImGuiColorEditFlags_NoInputs);
					ImGui::Combo("Material##backtrack", &Variables.VisualsChamsBacktrackMaterial, ChamsMaterials, ARRAYSIZE(ChamsMaterials));
					ImGui::SliderInt("Backtrack Alpha##chamsalpha", &Variables.VisualsChamsBacktrackAlpha, 0, 255);
				}
			}
			ImGui::PopItemWidth();
			ImGui::PopItemWidth();

			ImGui::NextColumn();

			ImGui::PushItemWidth(130);
			ImGui::Checkbox("Glow##glowenabled", &Variables.VisualsGlowEnabled);
			if (Variables.VisualsGlowEnabled)
			{
				ImGui::SameLine();
				ImGui::ColorEdit3("##glowcolor", Variables.VisualsGlowColor, ImGuiColorEditFlags_NoInputs);
				ImGui::Combo("Glowstyle##glow", &Variables.VisualsGlowGlowstyle, GlowStyles, ARRAYSIZE(GlowStyles));
				ImGui::SliderInt("Alpha##glowalpha", &Variables.VisualsGlowAlpha, 0, 255);
			}
			ImGui::PopItemWidth();
			break;
		case 3: // misc
			ImGui::Columns(2, "###visualtabcolumn", true);

			ImGui::PushItemWidth(130);

			ImGui::PopItemWidth();

			ImGui::NextColumn();

			ImGui::PushItemWidth(150);
			ImGui::Combo("Config File", &Variables.ConfigFile, ConfigFiles, ARRAYSIZE(ConfigFiles));
			if (ImGui::Button("Save", ImVec2(150, 18)))
				SaveConfig();
			if (ImGui::Button("Load", ImVec2(150, 18)))
				LoadConfig();
			ImGui::PopItemWidth();
			break;
		case 4: // skins
			ImGui::PushItemWidth(130);

			ImGui::PopItemWidth();
			break;
		}
		ImGui::EndChild();
	}
	ImGui::End();
}*/


void Menu::Show()
{
	Visible = true;
}

void Menu::Hide()
{
	Visible = false;
}

void Menu::Toggle()
{
	Visible = !Visible;
}

void Menu::CreateStyle()
{
	static int hue = 140;

	ImVec4 col_text = ImColor(150, 150, 150, 255);
	ImVec4 col_main = ImVec4(0.09f, .09f, .09f, 1.f);
	ImVec4 col_back = ImColor(31, 44, 54);
	ImVec4 col_area = ImColor(4, 32, 41);

	ImGui::GetIO().Fonts->AddFontDefault();

	Style.Alpha = 1.0f;                                // Global alpha applies to everything in ImGui
	Style.WindowPadding = ImVec2(7, 6);                      // Padding within a window
	Style.WindowMinSize = ImVec2(32, 32);                    // Minimum window size
	Style.WindowRounding = 4.0f;                                // Radius of window corners rounding. Set to 0.0f to have rectangular windows
	Style.WindowTitleAlign = ImVec2(0.006f, 0.5f);                  // Alignment for title bar text
	Style.FramePadding = ImVec2(4, 0);                        // Padding within a framed rectangle (used by most widgets)
	Style.FrameRounding = 10.0f;                                // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
	Style.ItemSpacing = ImVec2(8, 4);                        // Horizontal and vertical spacing between widgets/lines
	Style.ItemInnerSpacing = ImVec2(4, 4);                        // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
	Style.TouchExtraPadding = ImVec2(0, 0);                        // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
	Style.IndentSpacing = 21.0f;                               // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
	Style.ColumnsMinSpacing = 3.0f;                                // Minimum horizontal spacing between two columns
	Style.ScrollbarSize = 12.0f;                               // Width of the vertical scrollbar, Height of the horizontal scrollbar
	Style.ScrollbarRounding = 4.0f;                                // Radius of grab corners rounding for scrollbar
	Style.GrabMinSize = 10.0f;                               // Minimum width/height of a grab box for slider/scrollbar
	Style.GrabRounding = 10.0f;                                // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
	Style.ButtonTextAlign = ImVec2(0.5f, 0.5f);                  // Alignment of button text when button is larger than text.
	Style.DisplayWindowPadding = ImVec2(22, 22);                      // Window positions are clamped to be IsVisible within the display area by at least this amount. Only covers regular windows.
	Style.DisplaySafeAreaPadding = ImVec2(4, 4);                        // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
	Style.AntiAliasedLines = true;                                // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
	Style.CurveTessellationTol = 1.25f;                               // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
	Style.WindowBorderSize = 1.2f;

	Style.Colors[ImGuiCol_Text] = ImVec4(col_text.x, col_text.y, col_text.z, 1.f);
	Style.Colors[ImGuiCol_Text2] = ImColor(200, 80, 150, 255);
	Style.Colors[ImGuiCol_TextDisabled] = ImVec4(col_text.x, col_text.y, col_text.z, 1.f);
	Style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_text.x, col_text.y, col_text.z, 1.f);
	Style.Colors[ImGuiCol_WindowBg] = ImColor(9, 9, 9, 255);
	Style.Colors[ImGuiCol_ChildWindowBg] = ImColor(10, 10, 10, 255);
	Style.Colors[ImGuiCol_PopupBg] = ImColor(4, 4, 4, 255);
	Style.Colors[ImGuiCol_Border] = ImColor(25, 25, 25, 255);
	Style.Colors[ImGuiCol_BorderShadow] = ImColor(25, 25, 25, 255);
	Style.Colors[ImGuiCol_FrameBg] = ImColor(37, 40, 47, 255);
	Style.Colors[ImGuiCol_FrameBgHovered] = ImColor(37, 40, 47, 255);
	Style.Colors[ImGuiCol_FrameBgActive] = ImColor(37, 40, 47, 255);
	Style.Colors[ImGuiCol_TitleBg] = ImColor(37, 40, 47, 255);
	Style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(37, 40, 47, 255);
	Style.Colors[ImGuiCol_TitleBgActive] = ImColor(37, 40, 47, 255);
	Style.Colors[ImGuiCol_MenuBarBg] = ImColor(200, 80, 150, 255);
	Style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
	Style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(200, 80, 150, 255);
	Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(200, 80, 150, 255);
	Style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(200, 80, 150, 255);
	Style.Colors[ImGuiCol_CheckMark] = ImColor(200, 80, 150, 255);
	Style.Colors[ImGuiCol_SliderGrab] = ImColor(200, 80, 150, 255);
	Style.Colors[ImGuiCol_SliderGrabActive] = ImColor(200, 80, 150, 255);
	Style.Colors[ImGuiCol_Button] = ImColor(37, 40, 47, 255);
	Style.Colors[ImGuiCol_ButtonHovered] = ImColor(57, 60, 67, 255);
	Style.Colors[ImGuiCol_ButtonActive] = ImColor(57, 60, 67, 255);
	Style.Colors[ImGuiCol_Header] = ImColor(37, 40, 47, 170);
	Style.Colors[ImGuiCol_HeaderHovered] = ImColor(37, 40, 47, 170);
	Style.Colors[ImGuiCol_HeaderActive] = ImColor(37, 40, 47, 170);
	Style.Colors[ImGuiCol_Column] = ImVec4(col_text.x, col_text.y, col_text.z, 1.f);
	Style.Colors[ImGuiCol_ColumnHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 1.f);
	Style.Colors[ImGuiCol_ColumnActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.f);
	Style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
	Style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	Style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	Style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	Style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	Style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	Style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	Style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	ImGui::GetStyle() = Style;
}