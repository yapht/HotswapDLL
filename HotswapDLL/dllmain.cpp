#include <Windows.h>
#define CR_HOST
#include "../cr/cr.h"

#pragma warning(disable : 4996)

// Full path to the reloadable DLL
#define PLUGIN_PATH VS_SOLUTION_DIR"Debug\\HotswapPlugin.dll"

// The plugin does not need to update very frequently if this is going to be used in something like a cheat
#define PLUGIN_UPDATE_FREQ 2000

volatile bool run;

cr_plugin plugin;
HANDLE host_thread;

DWORD WINAPI HostThread(LPVOID)
{
    cr_plugin_open(plugin, PLUGIN_PATH);

    while (run && !cr_plugin_update(plugin))
        Sleep(1000);

    cr_plugin_close(plugin);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{

    DisableThreadLibraryCalls(hModule);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        run = true;

        AllocConsole();
        (void)freopen("conin$", "r", stdin);
        (void)freopen("conout$", "w", stdout);
        (void)freopen("conout$", "w", stderr);

        host_thread = CreateThread(NULL, NULL, HostThread, NULL, NULL, NULL);
    }

    if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        run = false;

        WaitForSingleObject(host_thread, INFINITE);   
        CloseHandle(host_thread);
    }

    return TRUE;
}