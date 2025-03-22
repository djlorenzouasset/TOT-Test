#pragma once
#include "framework.h"

inline CURLcode(*oCUrl_easy_setopt)(struct Curl_easy*, CURLoption, ...);

namespace CurlHook {
	inline std::ofstream output;

	inline uintptr_t CUrlEasySetOptAddr;
	inline const char* CUrlEasySetOptSig = "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC ? 33 ED 49 8B F0 48 8B D9";

	bool FindCurlEasy();

	CURLcode CUrlHook(struct Curl_easy* Handle, CURLoption Option, va_list Args);
}