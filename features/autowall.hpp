#pragma once
#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../singleton.hpp"

struct FireBulletData
{
	FireBulletData(const Vector& eye_pos)
		: src(eye_pos)
	{
	}

	Vector           src;
	trace_t          enter_trace;
	Vector           direction;
	CTraceFilter    filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};
#pragma once
class Autowall : public Singleton<Autowall>
{
public:

	void TraceLine(Vector& absStart, Vector& absEnd, unsigned int mask, IClientEntity* ignore, CGameTrace* ptr);
	void ClipTraceToPlayers(const Vector& absStart, const Vector absEnd, unsigned int mask, ITraceFilter* filter, CGameTrace* tr);
	void GetBulletTypeParameters(float& maxRange, float& maxDistance, char* bulletType, bool sv_penetration_type);
	bool BreakableEntity(IClientEntity* entity);
	void ScaleDamage(CGameTrace& enterTrace, CCSWeaponInfo* weaponData, float& currentDamage);
	bool trace_to_exit(CGameTrace& enterTrace, CGameTrace& exitTrace, Vector startPosition, Vector direction);
	bool HandleBulletPenetration(CCSWeaponInfo* weaponData, CGameTrace& enterTrace, Vector& eyePosition, Vector direction, int& possibleHitsRemaining, float& currentDamage, float penetrationPower, bool sv_penetration_type, float ff_damage_reduction_bullets, float ff_damage_bullet_penetration);
	bool FireBullet(C_BaseCombatWeapon* pWeapon, Vector& direction, float& currentDamage);
	float CanHit(Vector& point);
	float CanHit(Vector& start, Vector& point);
	float CanHit(C_BasePlayer* ent, Vector& point);
	bool trace_awall(float& damage);
	bool trace_awall(C_BasePlayer* m_local, Vector hit, float& damage);
	bool handle_penetration = false;

private:
	//typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	//void trace_line(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t &ptr);
};