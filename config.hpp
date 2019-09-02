#pragma once
#include "options.hpp"
#include "simpleini.hpp"
#include "menuarrays.hpp"
#include <iostream>
#include <fstream>

CSimpleIniA Config;

void SaveConfig()
{
	Config.SetUnicode(ConfigFiles[Variables.ConfigFile]);

	Config.SetBoolValue("Rage", "Aimbot", Variables.RageAimbotEnabled);
	Config.SetBoolValue("Rage", "Antiaim", Variables.RageAntiaimEnabled);
	Config.SetLongValue("Rage", "Hitchance", Variables.RageAimbotHitchance);
	Config.SetLongValue("Rage", "MinDmg", Variables.RageAimbotMinDmg);

	Config.SetBoolValue("Legit", "Aimbot", Variables.LegitAimbotEnabled);
	Config.SetLongValue("Legit", "Aimbot Type", Variables.LegitAimbotType);
	Config.SetLongValue("Legit", "Aimbot Hitbox", Variables.LegitAimbotHitbox);
	Config.SetDoubleValue("Legit", "Aimbot Fov", Variables.LegitAimbotFov);
	Config.SetLongValue("Legit", "Aimbot Smooth", Variables.LegitAimbotSmooth);
	Config.SetLongValue("Legit", "Aimbot Rcs", Variables.LegitAimbotRcs);

	Config.SetBoolValue("Legit", "Backtrack", Variables.LegitBacktrackEnabled);
	Config.SetLongValue("Legit", "Backtrack Duration", Variables.LegitBacktrackDuration);


	Config.SetBoolValue("Visuals", "Box", Variables.VisualsBox);
	Config.SetBoolValue("Visuals", "Health", Variables.VisualsHealth);
	Config.SetBoolValue("Visuals", "Name", Variables.VisualsName);
	Config.SetBoolValue("Visuals", "Weapon", Variables.VisualsWeapon);

	Config.SetBoolValue("Visuals", "Chams Enabled", Variables.VisualsChamsEnabled);
	Config.SetDoubleValue("Visuals", "Chams Red", Variables.VisualsChamsColor[0]);
	Config.SetDoubleValue("Visuals", "Chams Green", Variables.VisualsChamsColor[1]);
	Config.SetDoubleValue("Visuals", "Chams Blue", Variables.VisualsChamsColor[2]);
	Config.SetLongValue("Visuals", "Chams Material", Variables.VisualsChamsMaterial);
	Config.SetBoolValue("Visuals", "Chams IgnoreZ", Variables.VisualsChamsIgnoreZ);
	Config.SetDoubleValue("Visuals", "Chams IgnoreZ Red", Variables.VisualsChamsColorIgnoreZ[0]);
	Config.SetDoubleValue("Visuals", "Chams IgnoreZ Green", Variables.VisualsChamsColorIgnoreZ[1]);
	Config.SetDoubleValue("Visuals", "Chams IgnoreZ Blue", Variables.VisualsChamsColorIgnoreZ[2]);
	Config.SetLongValue("Visuals", "Chams Alpha", Variables.VisualsChamsAlpha);
	Config.SetLongValue("Visuals", "Backtrack Chams", Variables.VisualsChamsBacktrack);
	Config.SetLongValue("Visuals", "Backtrack Chams Material", Variables.VisualsChamsBacktrackMaterial);
	Config.SetLongValue("Visuals", "Backtrack Chams Alpha", Variables.VisualsChamsBacktrackAlpha);
	Config.SetDoubleValue("Visuals", "Chams Backtrack Red", Variables.VisualsChamsBacktrackColor[0]);
	Config.SetDoubleValue("Visuals", "Chams Backtrack Green", Variables.VisualsChamsBacktrackColor[1]);
	Config.SetDoubleValue("Visuals", "Chams Backtrack Blue", Variables.VisualsChamsBacktrackColor[2]);

	Config.SetBoolValue("Visuals", "Glow Enabled", Variables.VisualsGlowEnabled);
	Config.SetLongValue("Visuals", "Glow Style", Variables.VisualsGlowGlowstyle);
	Config.SetDoubleValue("Visuals", "Glow Red", Variables.VisualsGlowColor[0]);
	Config.SetDoubleValue("Visuals", "Glow Green", Variables.VisualsGlowColor[1]);
	Config.SetDoubleValue("Visuals", "Glow Blue", Variables.VisualsGlowColor[2]);
	Config.SetDoubleValue("Visuals", "Glow Alpha", Variables.VisualsGlowAlpha);

	Config.SetLongValue("Misc", "Fakelag Choke", Variables.MiscFakelagChoke);
	Config.SetBoolValue("Misc", "No Scope Border", Variables.VisualsNoScope);

	Config.SaveFile(ConfigFiles[Variables.ConfigFile]);
}
void LoadConfig()
{
	Config.SetUnicode(ConfigFiles[Variables.ConfigFile]);
	Config.LoadFile(ConfigFiles[Variables.ConfigFile]);

	Variables.RageAimbotEnabled = Config.GetBoolValue("Rage", "Aimbot", Variables.RageAimbotEnabled);
	Variables.RageAntiaimEnabled = Config.GetBoolValue("Rage", "Antiaim", Variables.RageAntiaimEnabled);
	Variables.RageAimbotHitchance = Config.GetLongValue("Rage", "Hitchance", Variables.RageAimbotHitchance);
	Variables.RageAimbotMinDmg = Config.GetLongValue("Rage", "MinDmg", Variables.RageAimbotMinDmg);

	Variables.LegitAimbotEnabled = Config.GetBoolValue("Legit", "Aimbot", Variables.LegitAimbotEnabled);
	Variables.LegitAimbotType = Config.GetLongValue("Legit", "Aimbot Type", Variables.LegitAimbotType);
	Variables.LegitAimbotHitbox = Config.GetLongValue("Legit", "Aimbot Hitbox", Variables.LegitAimbotHitbox);
	Variables.LegitAimbotFov = Config.GetDoubleValue("Legit", "Aimbot Fov", Variables.LegitAimbotFov);
	Variables.LegitAimbotSmooth = Config.GetLongValue("Legit", "Aimbot Smooth", Variables.LegitAimbotSmooth);
	Variables.LegitAimbotRcs = Config.GetLongValue("Legit", "Aimbot Rcs", Variables.LegitAimbotRcs);

	Variables.LegitBacktrackEnabled = Config.GetBoolValue("Legit", "Backtrack", Variables.LegitBacktrackEnabled);
	Variables.LegitBacktrackDuration = Config.GetLongValue("Legit", "Backtrack Duration", Variables.LegitBacktrackDuration);

	Variables.VisualsBox = Config.GetBoolValue("Visuals", "Box", Variables.VisualsBox);
	Variables.VisualsHealth = Config.GetBoolValue("Visuals", "Health", Variables.VisualsHealth);
	Variables.VisualsName = Config.GetBoolValue("Visuals", "Name", Variables.VisualsName);
	Variables.VisualsWeapon = Config.GetBoolValue("Visuals", "Weapon", Variables.VisualsWeapon);

	Variables.VisualsChamsEnabled = Config.GetBoolValue("Visuals", "Chams Enabled", Variables.VisualsChamsEnabled);
	Variables.VisualsChamsColor[0] = Config.GetDoubleValue("Visuals", "Chams Red", Variables.VisualsChamsColor[0]);
	Variables.VisualsChamsColor[1] = Config.GetDoubleValue("Visuals", "Chams Green", Variables.VisualsChamsColor[1]);
	Variables.VisualsChamsColor[2] = Config.GetDoubleValue("Visuals", "Chams Blue", Variables.VisualsChamsColor[2]);
	Variables.VisualsChamsMaterial = Config.GetLongValue("Visuals", "Chams Material", Variables.VisualsChamsMaterial);
	Variables.VisualsChamsIgnoreZ = Config.GetBoolValue("Visuals", "Chams IgnoreZ", Variables.VisualsChamsIgnoreZ);
	Variables.VisualsChamsColorIgnoreZ[0] = Config.GetDoubleValue("Visuals", "Chams IgnoreZ Red", Variables.VisualsChamsColorIgnoreZ[0]);
	Variables.VisualsChamsColorIgnoreZ[1] = Config.GetDoubleValue("Visuals", "Chams IgnoreZ Green", Variables.VisualsChamsColorIgnoreZ[1]);
	Variables.VisualsChamsColorIgnoreZ[2] = Config.GetDoubleValue("Visuals", "Chams IgnoreZ Blue", Variables.VisualsChamsColorIgnoreZ[2]);
	Variables.VisualsChamsAlpha = Config.GetLongValue("Visuals", "Chams Alpha", Variables.VisualsChamsAlpha);
	Variables.VisualsChamsBacktrack = Config.GetLongValue("Visuals", "Backtrack Chams", Variables.VisualsChamsBacktrack);
	Variables.VisualsChamsBacktrackMaterial = Config.GetLongValue("Visuals", "Backtrack Chams Material", Variables.VisualsChamsBacktrackMaterial);
	Variables.VisualsChamsBacktrackAlpha = Config.GetLongValue("Visuals", "Backtrack Chams Alpha", Variables.VisualsChamsBacktrackAlpha);
	Variables.VisualsChamsBacktrackColor[0] = Config.GetDoubleValue("Visuals", "Chams Backtrack Red", Variables.VisualsChamsBacktrackColor[0]);
	Variables.VisualsChamsBacktrackColor[1] = Config.GetDoubleValue("Visuals", "Chams Backtrack Green", Variables.VisualsChamsBacktrackColor[1]);
	Variables.VisualsChamsBacktrackColor[2] = Config.GetDoubleValue("Visuals", "Chams Backtrack Blue", Variables.VisualsChamsBacktrackColor[2]);

	Variables.VisualsGlowEnabled = Config.GetBoolValue("Visuals", "Glow Enabled", Variables.VisualsGlowEnabled);
	Variables.VisualsGlowGlowstyle = Config.GetLongValue("Visuals", "Glow Style", Variables.VisualsGlowGlowstyle);
	Variables.VisualsGlowColor[0] = Config.GetDoubleValue("Visuals", "Glow Red", Variables.VisualsGlowColor[0]);
	Variables.VisualsGlowColor[1] = Config.GetDoubleValue("Visuals", "Glow Green", Variables.VisualsGlowColor[1]);
	Variables.VisualsGlowColor[2] = Config.GetDoubleValue("Visuals", "Glow Blue", Variables.VisualsGlowColor[2]);
	Variables.VisualsGlowAlpha = Config.GetDoubleValue("Visuals", "Glow Alpha", Variables.VisualsGlowAlpha);

	Variables.MiscFakelagChoke = Config.GetLongValue("Misc", "Fakelag Choke", Variables.MiscFakelagChoke);
	Variables.VisualsNoScope = Config.GetBoolValue("Misc", "No Scope Border", Variables.VisualsNoScope);
}
