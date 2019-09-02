#pragma once
#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../singleton.hpp"
#include "../helpers/input.hpp"


class LegitBacktrack :
	public Singleton<LegitBacktrack>
{
	friend class Singleton<LegitBacktrack>;
private:
	struct TickInfo
	{
		TickInfo()
		{
			HeadPosition = Vector{};
			SimulationTime = -1;
			MatrixBuilt = false;
		}
		TickInfo(C_BasePlayer* Player)
		{
			HeadPosition = Player->GetHitboxPos(HITBOX_HEAD);
			SimulationTime = Player->m_flSimulationTime();
			MatrixBuilt = false;
			if (Player->SetupBones(BoneMatrix, 128, BONE_USED_BY_HITBOX, g_GlobalVars->curtime))
				MatrixBuilt = true;
		}
		Vector HeadPosition;
		float SimulationTime;
		bool MatrixBuilt;
		matrix3x4_t BoneMatrix[128];
	};
public:
	void Do(CUserCmd* cmd);
	std::vector<TickInfo> BacktrackRecords[65];
	int ClosestTick;
};
class LegitAimbot :
	public Singleton<LegitAimbot>
{
	friend class Singleton<LegitAimbot>;
private:
	int GetHitboxFromInt(int Hitbox);
public:
	void Do(CUserCmd* cmd, C_BaseCombatWeapon* Weapon);
};