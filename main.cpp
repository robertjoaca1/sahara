#define NOMINMAX
#include <Windows.h>

#include "valve_sdk/sdk.hpp"
#include "helpers/utils.hpp"

#include "hooks.hpp"
#include "options.hpp"
#include "helpers/input.hpp"
#include "features/menu.hpp"
/*
struct CvarInfo
{
	char* name;
	char* description;
	float min;
	float max;
	bool defaultval;
};
std::vector<CvarInfo> ConVars =
{
	CvarInfo { "cheat_legit_backtrack", "Legit Backtrack Enabled", 0.0, 1.0, false },
	CvarInfo { "cheat_chams_enabled", "Chams Enabled", 0.0, 1.0, false },
	CvarInfo { "cheat_chams_material", "Chams Material; 0 - No Material; 1 - Textured; 2 - Flat; 3 - Metallic; 4 - Pulsing;", 0.0, 4.0, true },
	CvarInfo { "cheat_chams_backtrack", "Chams Backtrack; 0 - Off; 1 - All Ticks; 2 - Last Tick", 0.0, 2.0, false },
	CvarInfo { "cheat_chams_r", "Chams Red", 0.0, 1.0, true },
	CvarInfo { "cheat_chams_g", "Chams Green", 0.0, 1.0, true },
	CvarInfo { "cheat_chams_b", "Chams Blue", 0.0, 1.0, true },
	CvarInfo { "cheat_chams_a", "Chams Alpha", 0.0, 1.0, true },
	CvarInfo { "cheat_chams_ignorez", "Chams IgnoreZ", 0.0, 1.0, false },
	CvarInfo { "cheat_chams_ignorez_r", "Chams IgnoreZ Red", 0.0, 1.0, true },
	CvarInfo { "cheat_chams_ignorez_g", "Chams IgnoreZ Green", 0.0, 1.0, true },
	CvarInfo { "cheat_chams_ignorez_b", "Chams IgnoreZ Blue", 0.0, 1.0, true },
	CvarInfo { "cheat_glow_enabled", "Glow Enabled", 0.0, 1.0, false },
	CvarInfo { "cheat_glow_r", "Glow Red", 0.0, 1.0, true },
	CvarInfo { "cheat_glow_g", "Glow Green", 0.0, 1.0, true },
	CvarInfo { "cheat_glow_b", "Glow Blue", 0.0, 1.0, true },
	CvarInfo { "cheat_glow_a", "Glow Alpha", 0.0, 1.0, true },
};
void CvarVariables()
{
	for (int i = 0; i < ConVars.size(); i++)
	{
		ConVar* RegisteredCvar = new ConVar(ConVars.at(i).name, ConVars.at(i).defaultval ? "1.0" : "0.0", FCVAR_RELEASE, ConVars.at(i).description, true, ConVars.at(i).min, true, ConVars.at(i).max);
		g_CVar->RegisterConCommand(RegisteredCvar);
	}
}*/

bool Unload = false;
DWORD WINAPI OnDllAttach(LPVOID base)
{
    // 
    // Wait at most 10s for the main game modules to be loaded.
    // 
    if(Utils::WaitForModules(10000, { L"client_panorama.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT) {
        // One or more modules were not loaded in time
        return FALSE;
    }

#ifdef _DEBUG
    Utils::AttachConsole();
#endif

    try {
        Utils::ConsolePrint("Initializing...\n");

        Interfaces::Initialize();
        Interfaces::Dump();

		//CvarVariables();

        NetvarSys::Get().Initialize();
		InputSys::Get().Initialize();
		Menu::Get().Initialize();
        Hooks::Initialize();


		InputSys::Get().RegisterHotkey(VK_INSERT, [base]() {
			Menu::Get().Toggle();
			});
		InputSys::Get().RegisterHotkey(VK_END, [base]() {
			Unload = true;
			});
        // Register some hotkeys.
        // - Note:  The function that is called when the hotkey is pressed
        //          is called from the WndProc thread, not this thread.
        // 

        Utils::ConsolePrint("Finished.\n");

        while(!Unload)
            Sleep(1000);

        g_CVar->FindVar("crosshair")->SetValue(true);

        FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);

    } catch(const std::exception& ex) {
        Utils::ConsolePrint("An error occured during initialization:\n");
        Utils::ConsolePrint("%s\n", ex.what());
        Utils::ConsolePrint("Press any key to exit.\n");
        Utils::ConsoleReadKey();
        Utils::DetachConsole();

        FreeLibraryAndExitThread(static_cast<HMODULE>(base), 1);
    }

    // unreachable
    //return TRUE;
}

BOOL WINAPI OnDllDetach()
{
#ifdef _DEBUG
    Utils::DetachConsole();
#endif

    Hooks::Shutdown();
	Menu().Shutdown();
    return TRUE;
}

BOOL WINAPI DllMain(
    _In_      HINSTANCE hinstDll,
    _In_      DWORD     fdwReason,
    _In_opt_  LPVOID    lpvReserved
)
{
    switch(fdwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDll);
            CreateThread(nullptr, 0, OnDllAttach, hinstDll, 0, nullptr);
            return TRUE;
        case DLL_PROCESS_DETACH:
            if(lpvReserved == nullptr)
                return OnDllDetach();
            return TRUE;
        default:
            return TRUE;
    }
}
