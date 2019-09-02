#pragma once
#include "../options.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../helpers/utils.hpp"
#include "../singleton.hpp"


class Render
	: public Singleton<Render>
{
	friend class Singleton<Render>;
private:

public:
	vgui::HFont VerdanaBold12;
	void CreateFonts();
	void Text(int X, int Y, const char* Text, vgui::HFont Font, Color DrawColor, bool Center);
	void TextSize(int& Width, int& Height, const char* Text, vgui::HFont Font);
	void FilledRectange(int X1, int Y1, int X2, int Y2, Color DrawColor);
	void OutlinedRectange(int X1, int Y1, int X2, int Y2, Color DrawColor);
	void Line(int X1, int Y1, int X2, int Y2, Color DrawColor);
};
class Chams
	: public Singleton<Chams>
{
	friend class Singleton<Chams>;
	Chams();
	~Chams();

public:
	void OnDrawModelExecute(
		IMatRenderContext* ctx,
		const DrawModelState_t& state,
		const ModelRenderInfo_t& pInfo,
		matrix3x4_t* pCustomBoneToWorld);
private:

	IMaterial* materialRegular = nullptr;
	IMaterial* materialRegularIgnoreZ = nullptr;
	IMaterial* materialFlat = nullptr;
	IMaterial* materialFlatIgnoreZ = nullptr;
	IMaterial* materialMetallic = nullptr;
	IMaterial* materialMetallicIgnoreZ = nullptr;
	IMaterial* materialDogtag = nullptr;
};

class Glow
	: public Singleton<Glow>
{
	friend class Singleton<Glow>;

	Glow();
	~Glow();

public:
	void Run();
	void Shutdown();
};

class Visuals
	: public Singleton<Visuals>
{
	friend class Singleton<Visuals>;
private:
	RECT GetBBox(C_BasePlayer* Player, Vector TransformedPoints[]);
	struct
	{
		C_BasePlayer* Player;
		RECT Box;
		Vector HeadPos;
		Vector Origin;
	} Context;
public:
	bool Begin(C_BasePlayer* Player);
	void Box();
	void Name();
	void Weapon();
	void Health();
};