#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "valve_sdk/Misc/Color.hpp"

class Options
{
public:
	//legit
	bool LegitAimbotEnabled = false;
	int LegitAimbotType = 0;
	int LegitAimbotHitbox = 0;
	float LegitAimbotFov = 0.0f;
	int LegitAimbotSmooth = 0;
	int LegitAimbotRcs = 0;

	bool LegitBacktrackEnabled = false;
	int LegitBacktrackDuration = 200;
	//rage
	bool RageAimbotEnabled = false;
	int RageAimbotHitchance = 0;
	int RageAimbotMinDmg = 0;
	bool RageAntiaimEnabled = false;
	//visuals
	bool VisualsBox = false;
	bool VisualsName = false;
	bool VisualsWeapon = false;
	bool VisualsHealth = false;

	bool VisualsChamsEnabled = false;
	float VisualsChamsColor[3] = { 1.f, 1.f, 1.f };
	int VisualsChamsMaterial = 2;
	bool VisualsChamsIgnoreZ = false;
	float VisualsChamsColorIgnoreZ[3] = { 1.f, 1.f, 1.f };
	int VisualsChamsAlpha = 255;
	int VisualsChamsBacktrack = 0;
	int VisualsChamsBacktrackMaterial = 2;
	float VisualsChamsBacktrackColor[3] = { 1.f, 1.f, 1.f };
	int VisualsChamsBacktrackAlpha = 255;

	bool VisualsGlowEnabled = false;
	int VisualsGlowGlowstyle = 0;
	float VisualsGlowColor[3] = { 1.f, 1.f, 1.f };
	int VisualsGlowAlpha = 255;
	//misc
	int MiscFakelagChoke = 0;
	bool VisualsNoScope = false;
	//skins

	//menu
	int ConfigFile = 0;
};
inline Options Variables;
