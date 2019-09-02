#include "legitbot.hpp"

void LegitBacktrack::Do(CUserCmd* cmd)
{
	if (!g_LocalPlayer || 
		!g_LocalPlayer->IsAlive())
		return; 

	int BacktrackedPlayer = -1;
	float MaxPlayerFov = INT_MAX;

	for (int i = 1; i <= 64; i++)
	{
		C_BasePlayer* Player = C_BasePlayer::GetPlayerByIndex(i);
		if (!Player ||
			Player->IsDormant() ||
			!Player->IsPlayer() ||
			!Player->IsAlive() ||
			!Player->IsEnemy())
		{
			if (BacktrackRecords[i].size() > 0)
				for (int Tick = 0; Tick < BacktrackRecords[i].size(); Tick++)
					BacktrackRecords[i].erase(BacktrackRecords[i].begin() + Tick);
			continue;
		}

		BacktrackRecords[i].insert(BacktrackRecords[i].begin(), TickInfo(Player));
		for (auto Tick : BacktrackRecords[i])
			if (!Utils::IsTickValid(Tick.SimulationTime, float(Variables.LegitBacktrackDuration) / 1000.f))
				BacktrackRecords[i].pop_back();

		Vector ViewAngles;
		Math::AngleVectors(cmd->viewangles + (g_LocalPlayer->m_aimPunchAngle() * g_CVar->FindVar("weapon_recoil_scale")->GetFloat()), ViewAngles);
		float FovDistance = Math::DistancePointToLine(Player->GetHitboxPos(HITBOX_HEAD), g_LocalPlayer->GetEyePos(), ViewAngles);

		if (MaxPlayerFov > FovDistance)
		{
			MaxPlayerFov = FovDistance;
			BacktrackedPlayer = i;
		}
	}
	ClosestTick = -1;
	float MaxTickFov = INT_MAX;
	if (BacktrackedPlayer != -1)
	{
		for (int t = 0; t < BacktrackRecords[BacktrackedPlayer].size(); t++)
		{
			Vector ViewAngles2;
			Math::AngleVectors(cmd->viewangles + (g_LocalPlayer->m_aimPunchAngle() * g_CVar->FindVar("weapon_recoil_scale")->GetFloat()), ViewAngles2);
			float FovDistance2 = Math::DistancePointToLine(BacktrackRecords[BacktrackedPlayer].at(t).HeadPosition, g_LocalPlayer->GetEyePos(), ViewAngles2);

			if (MaxTickFov > FovDistance2)
			{
				MaxTickFov = FovDistance2;
				ClosestTick = t;
			}
		}
		//if (ClosestTick != -1 && BacktrackRecords[BacktrackedPlayer].at(ClosestTick).SimulationTime != -1)
		//	cmd->tick_count = TIME_TO_TICKS(BacktrackRecords[BacktrackedPlayer].at(ClosestTick).SimulationTime + GetLerpTime());
	}
}
void LegitAimbot::Do(CUserCmd* cmd, C_BaseCombatWeapon* Weapon)
{
	if (!g_LocalPlayer ||
		!g_LocalPlayer->IsAlive() ||
		!Weapon ||
		Weapon->IsKnife() ||
		Weapon->IsGrenade() ||
		!Variables.LegitAimbotEnabled ||
		!Variables.LegitAimbotFov)
		return;

	float MaxPlayerFov = Variables.LegitAimbotFov;
	int ClosestPlayerIndex = -1;
	QAngle AimAngle = QAngle{};
	QAngle ViewAngle = QAngle{};
	g_EngineClient->GetViewAngles(ViewAngle);

	for (int i = 1; i <= 64; i++)
	{
		C_BasePlayer* Player = C_BasePlayer::GetPlayerByIndex(i);
		if (!Player ||
			!Player->IsPlayer() ||
			Player->IsDormant() ||
			!Player->IsAlive() ||
			!Player->IsEnemy())
			continue;

		Vector Hitbox;
		bool Backtrack = false;
		if (Variables.LegitBacktrackEnabled && LegitBacktrack::Get().ClosestTick != -1 && LegitBacktrack::Get().ClosestTick < LegitBacktrack::Get().BacktrackRecords[i].size() && LegitBacktrack::Get().BacktrackRecords[i].at(LegitBacktrack::Get().ClosestTick).MatrixBuilt)
		{
			Hitbox = Player->GetHitboxPos(GetHitboxFromInt(Variables.LegitAimbotHitbox),
				LegitBacktrack::Get().BacktrackRecords[i].at(LegitBacktrack::Get().ClosestTick).BoneMatrix);
			
			Backtrack = true;
		}
		if (!Backtrack)
			Hitbox = Player->GetHitboxPos(GetHitboxFromInt(Variables.LegitAimbotHitbox));

		float FovDistance = Math::GetFOV(ViewAngle + (g_LocalPlayer->m_aimPunchAngle() * g_CVar->FindVar("weapon_recoil_scale")->GetFloat()), Math::CalcAngle(g_LocalPlayer->GetEyePos(), Hitbox));

		if (MaxPlayerFov > FovDistance)
		{
			if (g_LocalPlayer->CanSeePlayer(Player, Hitbox))
			{
				MaxPlayerFov = FovDistance;
				ClosestPlayerIndex = i;
			}
		}
	}
	if (ClosestPlayerIndex != -1)
	{
		C_BasePlayer* ClosestPlayer = C_BasePlayer::GetPlayerByIndex(ClosestPlayerIndex);
		if (!ClosestPlayer) return;
		Vector Hitbox;
		bool Backtrack = false;
		if (Variables.LegitBacktrackEnabled && LegitBacktrack::Get().ClosestTick != -1 && LegitBacktrack::Get().ClosestTick < LegitBacktrack::Get().BacktrackRecords[ClosestPlayerIndex].size() && LegitBacktrack::Get().BacktrackRecords[ClosestPlayerIndex].at(LegitBacktrack::Get().ClosestTick).MatrixBuilt)
		{
			Hitbox = ClosestPlayer->GetHitboxPos(GetHitboxFromInt(Variables.LegitAimbotHitbox),
				LegitBacktrack::Get().BacktrackRecords[ClosestPlayerIndex].at(LegitBacktrack::Get().ClosestTick).BoneMatrix);

			Backtrack = true;
		}
		if (!Backtrack)
			Hitbox = ClosestPlayer->GetHitboxPos(GetHitboxFromInt(Variables.LegitAimbotHitbox));
		AimAngle = Math::CalcAngle(g_LocalPlayer->GetEyePos(), Hitbox);
		AimAngle -= (g_LocalPlayer->m_aimPunchAngle() * g_CVar->FindVar("weapon_recoil_scale")->GetFloat()) * (float(Variables.LegitAimbotRcs) / 100.f);
		Math::Normalize3(AimAngle);
		Math::ClampAngles(AimAngle);
		QAngle DeltaAngle = ViewAngle - AimAngle;
		Math::Normalize3(DeltaAngle);
		Math::ClampAngles(DeltaAngle);
		float Smoothing = (Variables.LegitAimbotType == 1 && Variables.LegitAimbotSmooth > 1) ? Variables.LegitAimbotSmooth : 1;
		QAngle FinalAngle = ViewAngle - DeltaAngle / Smoothing;
		Math::Normalize3(FinalAngle);
		Math::ClampAngles(FinalAngle);
		if (Backtrack)
		{
			cmd->tick_count = TIME_TO_TICKS(LegitBacktrack::Get().BacktrackRecords[ClosestPlayerIndex].at(LegitBacktrack::Get().ClosestTick).SimulationTime + Utils::GetLerpTime());
		}
		cmd->viewangles = FinalAngle;
		if (Variables.LegitAimbotType != 2)
			g_EngineClient->SetViewAngles(cmd->viewangles);

		/*if (!(cmd->buttons & IN_ATTACK) && Weapon->CanFire())
		{
			//globals::shots_fired[entity->EntIndex()] += 1;
			cmd->buttons |= IN_ATTACK;
		}/*/
	}
}
int LegitAimbot::GetHitboxFromInt(int Hitbox)
{
	if (Hitbox != HITBOX_HEAD && Hitbox != HITBOX_NECK)
		return HITBOX_CHEST;
	
	return Hitbox;
}