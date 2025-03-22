#pragma once

#define WIN32_LEAN_AND_MEAN

#include <fstream>
#include <iostream>
#include <windows.h>
#include <filesystem>

#pragma comment(lib, "Include/minhook/minhook.lib")

#include "memcury.h"
#include "curl/curl.h"
#include "minhook/minhook.h"

#define TO_ADDR(addr) \
	(addr - reinterpret_cast<uintptr_t>(GetModuleHandle(0)))

#define CREATE_HOOK(addr, detour, original) \
	MH_CreateHook(addr, detour, original); \
	MH_EnableHook(addr);

#define REMOVE_HOOK(addr) \
	MH_RemoveHook(addr);