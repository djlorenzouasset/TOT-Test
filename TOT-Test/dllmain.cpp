#include "pch.h"
#include "CurlHook.h"

DWORD MainThread(HMODULE hModule)
{
    AllocConsole();
    SetConsoleTitleA("TOT-Sniffer");

    FILE* Dummy;
    freopen_s(&Dummy, "CONOUT$", "w", stderr); // we use stderr for don't redirect the game logging on our console

    std::filesystem::path outfile_path("C:/TOT/");
    if (!std::filesystem::exists(outfile_path))
    {
        std::filesystem::create_directories(outfile_path);
    }

    CurlHook::output.open(outfile_path / "urls_dump.txt");

    if (MH_Initialize() != MH_OK) {
        std::cerr << "Failed to initialize MinHook!";
        MessageBoxA(nullptr, "Failed to initialize MinHook!", "Error", MB_ICONERROR | MB_OK);
        goto keybind;
    }

    if (!CurlHook::FindCurlEasy())
    {
        std::cerr << "Failed to find Curl_easy_setopt()!!\n";
        MessageBoxA(nullptr, "Failed to find Curl_easy_setopt()!!", "Error", MB_ICONERROR | MB_OK);
        goto keybind;
    }

    CREATE_HOOK((void*)CurlHook::CUrlEasySetOptAddr, CurlHook::CUrlHook, (void**)&oCUrl_easy_setopt);
    std::cerr << "Hooked Curl_easy_setopt().\n";

keybind:
    while (true)
    {
        if (GetAsyncKeyState(VK_F6) & 1)
        {
            if (CurlHook::CUrlEasySetOptAddr)
            {
                REMOVE_HOOK((void*)CurlHook::CUrlEasySetOptAddr);
            }

            CurlHook::output.close();

            fclose(stdout);
            if (Dummy) fclose(Dummy);

            FreeConsole();
            FreeLibraryAndExitThread(hModule, 0);
        }

        Sleep(100);
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH) 
    {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0);
    }

    return TRUE;
}

