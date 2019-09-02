#include "hooks.hpp"
#include <intrin.h>  
#include "helpers/input.hpp"
#include "options.hpp"
#include "helpers/utils.hpp"
#include "features/visuals.hpp"
#include "features/legitbot.hpp"
#include "features/ragebot.hpp"
#include "features/menu.hpp"

#pragma intrinsic(_ReturnAddress) 
float Hooks::RealAngle;
float Hooks::FakeAngle;
float Hooks::Pitch;
namespace Hooks {

	void Initialize()
	{
		direct3d_hook.setup(g_D3DDevice9);
		hlclient_hook.setup(g_CHLClient);
		vguipanel_hook.setup(g_VGuiPanel);
		vguisurf_hook.setup(g_VGuiSurface);
		mdlrender_hook.setup(g_MdlRender);
		clientmode_hook.setup(g_ClientMode);
		ConVar* sv_cheats_con = g_CVar->FindVar("sv_cheats");
		sv_cheats.setup(sv_cheats_con);

		direct3d_hook.hook_index(index::EndScene, hkEndScene);
		direct3d_hook.hook_index(index::Reset, hkReset);
		hlclient_hook.hook_index(index::FrameStageNotify, hkFrameStageNotify);
		hlclient_hook.hook_index(index::CreateMove, hkCreateMove_Proxy);
		vguipanel_hook.hook_index(index::PaintTraverse, hkPaintTraverse);
		vguisurf_hook.hook_index(index::LockCursor, hkLockCursor);
		mdlrender_hook.hook_index(index::DrawModelExecute, hkDrawModelExecute);
		clientmode_hook.hook_index(index::DoPostScreenSpaceEffects, hkDoPostScreenEffects);
		clientmode_hook.hook_index(index::OverrideView, hkOverrideView);
		sv_cheats.hook_index(index::SvCheatsGetBool, hkSvCheatsGetBool);

		Render::Get().CreateFonts();

		g_InputSystem->EnableInput(true);

		static ConVar* postprocess = g_CVar->FindVar("mat_postprocess_enable");
		postprocess->m_fnChangeCallbacks = 0;
		postprocess->SetValue(0);
	}
	//--------------------------------------------------------------------------------
	void Shutdown()
	{
		direct3d_hook.unhook_all();
		hlclient_hook.unhook_all();
		vguipanel_hook.unhook_all();
		vguisurf_hook.unhook_all();
		mdlrender_hook.unhook_all();
		clientmode_hook.unhook_all();
		sv_cheats.unhook_all(); 
		Glow::Get().Shutdown();

		g_InputSystem->EnableInput(true);
	}
	//--------------------------------------------------------------------------------
	long __stdcall hkEndScene(IDirect3DDevice9* pDevice)
	{
		static auto oEndScene = direct3d_hook.get_original<decltype(&hkEndScene)>(index::EndScene);

		DWORD colorwrite, srgbwrite;
		pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
		pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
		//removes the source engine color correction
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
		pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);
		
		Menu::Get().RenderMenu();

		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);

		return oEndScene(pDevice);
	}
	//--------------------------------------------------------------------------------
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		static auto oReset = direct3d_hook.get_original<decltype(&hkReset)>(index::Reset);

		Menu::Get().OnDeviceLost();

		auto hr = oReset(device, pPresentationParameters);

		if (hr >= 0)
		{
			Menu::Get().OnDeviceReset();
			Render::Get().CreateFonts();
		}

		return hr;
	}
	//--------------------------------------------------------------------------------

	void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
	{
		static auto oCreateMove = hlclient_hook.get_original<decltype(&hkCreateMove_Proxy)>(index::CreateMove);

		oCreateMove(g_CHLClient, 0, sequence_number, input_sample_frametime, active);

		auto cmd = g_Input->GetUserCmd(sequence_number);
		auto verified = g_Input->GetVerifiedCmd(sequence_number);

		if (!cmd || !cmd->command_number)
			return;

		C_BaseCombatWeapon* Weapon = g_LocalPlayer->m_hActiveWeapon();

		bSendPacket = g_EngineClient->GetNetChannel()->m_nChokedPackets >= Variables.MiscFakelagChoke;
		if (cmd->buttons & IN_ATTACK)
			bSendPacket = true;

		MovementFix::Get().Start(cmd);

		RageAimbot::Get().StartEnginePred(cmd);

		if (Variables.RageAimbotEnabled)
		{
			Variables.LegitAimbotEnabled = false;
			Variables.LegitBacktrackEnabled = false;
			RageAimbot::Get().StoreRecords();
			RageAimbot::Get().Do(cmd, Weapon, bSendPacket);
			if (Variables.RageAntiaimEnabled)
				RageAimbot::Get().DoAntiaim(cmd, Weapon, bSendPacket);
		}
		else
		{
			if (Variables.LegitBacktrackEnabled)
				LegitBacktrack::Get().Do(cmd);

			if (Variables.LegitAimbotEnabled)
				LegitAimbot::Get().Do(cmd, Weapon);
		}

		RageAimbot::Get().EndEnginePred();

		MovementFix::Get().End(cmd);
		Math::Normalize3(cmd->viewangles);
		Math::ClampAngles(cmd->viewangles);

		if (bSendPacket)
			Hooks::FakeAngle = cmd->viewangles.yaw;
		else
			Hooks::RealAngle = cmd->viewangles.yaw;

		Hooks::Pitch = cmd->viewangles.pitch;

		verified->m_cmd = *cmd;
		verified->m_crc = cmd->GetChecksum();
	}
	//--------------------------------------------------------------------------------
	__declspec(naked) void __fastcall hkCreateMove_Proxy(void* _this, int, int sequence_number, float input_sample_frametime, bool active)
	{
		__asm
		{
			push ebp
			mov  ebp, esp
			push ebx
			lea  ecx, [esp]
			push ecx
			push dword ptr[active]
			push dword ptr[input_sample_frametime]
			push dword ptr[sequence_number]
			call Hooks::hkCreateMove
			pop  ebx
			pop  ebp
			retn 0Ch
		}
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkPaintTraverse(void* _this, int edx, vgui::VPANEL panel, bool forceRepaint, bool allowForce)
	{
		static auto panelId = vgui::VPANEL{ 0 };
		static auto oPaintTraverse = vguipanel_hook.get_original<decltype(&hkPaintTraverse)>(index::PaintTraverse);

		if (Variables.VisualsNoScope && strcmp("HudZoom", g_VGuiPanel->GetName(panel)) == 0)
			return;

		oPaintTraverse(g_VGuiPanel, edx, panel, forceRepaint, allowForce);

		if (!panelId) {
			const auto panelName = g_VGuiPanel->GetName(panel);
			if (!strcmp(panelName, "FocusOverlayPanel")) {
				panelId = panel;
			}
		}
		else if (panelId == panel) {
			int ScreenWidth, ScreenHeight;
			g_EngineClient->GetScreenSize(ScreenWidth, ScreenHeight);
			Render::Get().Text(1, 1, "Sahara csgo", Render::Get().VerdanaBold12, Color(255, 255, 255, 255), false);

			if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected() || !g_LocalPlayer)
				return;

			C_BaseCombatWeapon* Weapon = g_LocalPlayer->m_hActiveWeapon();

			if (InputSys::Get().IsKeyDown(VK_TAB))
				Utils::RankRevealAll();

			if (Weapon && Variables.VisualsNoScope && Weapon->IsSniper() && g_LocalPlayer->m_bIsScoped())
			{
				Render::Get().Line(ScreenWidth / 2, 0, ScreenWidth / 2, ScreenHeight, Color(0, 0, 0, 150));
				Render::Get().Line(0, ScreenHeight / 2, ScreenWidth, ScreenHeight / 2, Color(0, 0, 0, 150));
			}

			for (int i = 1; i <= 64; i++) 
			{
				auto Player = C_BasePlayer::GetPlayerByIndex(i);

				if (!Player)
					continue;

				//if (entity == g_LocalPlayer)
				//	continue;

				if (i < 65 && Player->IsAlive())
				{
					if (Visuals::Get().Begin(Player)) 
					{
						if (Variables.VisualsBox)
							Visuals::Get().Box();
						if (Variables.VisualsHealth)
							Visuals::Get().Health();
						if (Variables.VisualsName)
							Visuals::Get().Name();
						if (Variables.VisualsWeapon)
							Visuals::Get().Weapon();
					}
				}
			}
		}
	}
	//--------------------------------------------------------------------------------
	int __fastcall hkDoPostScreenEffects(void* _this, int edx, int a1)
	{
		static auto oDoPostScreenEffects = clientmode_hook.get_original<decltype(&hkDoPostScreenEffects)>(index::DoPostScreenSpaceEffects);
		
		Glow::Get().Run();

		return oDoPostScreenEffects(g_ClientMode, edx, a1);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkFrameStageNotify(void* _this, int edx, ClientFrameStage_t stage)
	// i love pasting yes
	{
		static auto ofunc = hlclient_hook.get_original<decltype(&hkFrameStageNotify)>(index::FrameStageNotify);
		if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected() || !g_LocalPlayer) // checking for this instead of g_LocalPlayer coz it fucks up when full updating
		{
			ofunc(g_CHLClient, edx, stage);
			return;
		}
		if (stage == FRAME_RENDER_START)
		{
			for (int i = 1; i <= 64; i++)
			{
				C_BasePlayer* Player = C_BasePlayer::GetPlayerByIndex(i);
				if (!Player || !Player->IsPlayer() || Player == g_LocalPlayer) continue;

				*(int*)((uintptr_t)Player + 0xA30) = g_GlobalVars->framecount;
				*(int*)((uintptr_t)Player + 0xA28) = 0;
			}

			static auto deadflag = NetvarSys::Get().GetOffset("DT_BasePlayer", "deadflag");
			if (g_LocalPlayer->IsAlive())
			{
				if (GetKeyState(VK_MBUTTON))
				{
					*(bool*)((DWORD)g_Input + 0xAD) = true;
					*(float*)((DWORD)g_Input + 0xA8 + 0x8 + 0x8) = 180;
					*(QAngle*)((DWORD)g_LocalPlayer.operator->() + deadflag + 0x4) = QAngle{ Hooks::Pitch, Hooks::RealAngle, 0 };
				}
				else
				{
					*(bool*)((DWORD)g_Input + 0xAD) = false;
					*(float*)((DWORD)g_Input + 0xA8 + 0x8 + 0x8) = 0;
				}
			}
			else
			{
				*(bool*)((DWORD)g_Input + 0xAD) = false;
				*(float*)((DWORD)g_Input + 0xA8 + 0x8 + 0x8) = 0;
			}
		}
		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_END)
		{
			for (int i = 1; i <= 64; i++)
			{
				C_BasePlayer* Player = C_BasePlayer::GetPlayerByIndex(i);
				if (!Player
					|| !Player->IsAlive())
					continue;
				if (Player->IsDormant())
					continue;

				auto varmap = reinterpret_cast<uintptr_t>(Player) + 36;
				auto varmap_size = *reinterpret_cast<int*>(varmap + 20);

				for (auto index = 0; index < varmap_size; index++)
					* reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(varmap) + index * 12) = 0;
			}
		}

		ofunc(g_CHLClient, edx, stage);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkOverrideView(void* _this, int edx, CViewSetup* vsView)
	{
		static auto ofunc = clientmode_hook.get_original<decltype(&hkOverrideView)>(index::OverrideView);
		if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		{
			ofunc(g_ClientMode, edx, vsView);
			return;
		}
		vsView->fov = 90.f; //yes

		ofunc(g_ClientMode, edx, vsView);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkLockCursor(void* _this)
	{
		static auto ofunc = vguisurf_hook.get_original<decltype(&hkLockCursor)>(index::LockCursor);

		if (Menu::Get().IsVisible()) {
			g_VGuiSurface->UnlockCursor();
			g_InputSystem->ResetInputState();
			return;
		}
		ofunc(g_VGuiSurface);
	}
	//--------------------------------------------------------------------------------
	void __fastcall hkDrawModelExecute(void* _this, int edx, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{
		static auto ofunc = mdlrender_hook.get_original<decltype(&hkDrawModelExecute)>(index::DrawModelExecute);

		Chams::Get().OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);
	}
	bool __fastcall hkSvCheatsGetBool(PVOID pConVar, void* edx)
	{
		static auto dwCAM_Think = Utils::PatternScan(GetModuleHandleW(L"client_panorama.dll"), "85 C0 75 30 38 86");
		static auto ofunc = sv_cheats.get_original<bool(__thiscall *)(PVOID)>(13);

		if (!ofunc)
			return false;

		if (reinterpret_cast<DWORD>(_ReturnAddress()) == reinterpret_cast<DWORD>(dwCAM_Think))
			return true;
		return ofunc(pConVar);
	}
}
