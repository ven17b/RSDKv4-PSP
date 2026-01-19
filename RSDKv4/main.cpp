#include "RetroEngine.hpp"

#if RETRO_PLATFORM == RETRO_PSP
#include <pspkernel.h>
#include <psppower.h>

volatile bool pspSuspended = false;

int pspExitCallback(int arg1, int arg2, void *common) {
    Engine.running = false;
    return 0;
}

int pspPowerCallback(int unknown, int pwrflags, void *common) {
    if (pwrflags & PSP_POWER_CB_POWER_SWITCH || pwrflags & PSP_POWER_CB_SUSPENDING) {
        pspSuspended = true;
    } else if (pwrflags & PSP_POWER_CB_RESUMING || pwrflags & PSP_POWER_CB_RESUME_COMPLETE) {
        pspSuspended = false;
    }
    return 0;
}

int pspCallbackThread(SceSize args, void *argp) {
    int cbid;
    
    cbid = sceKernelCreateCallback("Exit Callback", pspExitCallback, NULL);
    sceKernelRegisterExitCallback(cbid);
    
    cbid = sceKernelCreateCallback("Power Callback", pspPowerCallback, NULL);
    scePowerRegisterCallback(0, cbid);
    
    sceKernelSleepThreadCB();
    return 0;
}

void pspSetupCallbacks() {
    int thid = sceKernelCreateThread("psp_callback_thread", pspCallbackThread, 0x11, 0xFA0, 0, NULL);
    if (thid >= 0) {
        sceKernelStartThread(thid, 0, NULL);
    }
}
#endif

#if !RETRO_USE_ORIGINAL_CODE

#if RETRO_PLATFORM == RETRO_WIN
#include "Windows.h"
#endif

void parseArguments(int argc, char *argv[])
{
    for (int a = 0; a < argc; ++a) {
        const char *find = "";

        find = strstr(argv[a], "stage=");
        if (find) {
            int b = 0;
            int c = 6;
            while (find[c] && find[c] != ';') Engine.startSceneFolder[b++] = find[c++];
            Engine.startSceneFolder[b] = 0;
        }

        find = strstr(argv[a], "scene=");
        if (find) {
            int b = 0;
            int c = 6;
            while (find[c] && find[c] != ';') Engine.startSceneID[b++] = find[c++];
            Engine.startSceneID[b] = 0;
        }

        find = strstr(argv[a], "console=true");
        if (find) {
            engineDebugMode       = true;
            Engine.devMenu        = true;
            Engine.consoleEnabled = true;
#if RETRO_PLATFORM == RETRO_WIN
            AllocConsole();
            freopen_s((FILE **)stdin, "CONIN$", "w", stdin);
            freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
            freopen_s((FILE **)stderr, "CONOUT$", "w", stderr);
#endif
        }

        find = strstr(argv[a], "usingCWD=true");
        if (find) {
            usingCWD = true;
        }
    }
}
#endif

int main(int argc, char *argv[])
{
#if RETRO_PLATFORM == RETRO_PSP
    pspSetupCallbacks();
#endif

#if !RETRO_USE_ORIGINAL_CODE
    parseArguments(argc, argv);
#endif

#if RETRO_PLATFORM == RETRO_UWP
    SDL_SetHint(SDL_HINT_WINRT_HANDLE_BACK_BUTTON, "1");
#endif
    Engine.Init();
    Engine.Run();

#if !RETRO_USE_ORIGINAL_CODE
    if (Engine.consoleEnabled) {
#if RETRO_PLATFORM == RETRO_WIN
        FreeConsole();
#endif
    }
#endif

#if RETRO_PLATFORM == RETRO_PSP
    sceKernelExitGame();
#endif

    return 0;
}

#if RETRO_PLATFORM == RETRO_UWP
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) { return SDL_WinRTRunApp(main, NULL); }
#endif