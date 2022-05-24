#include "pch.h"


bool hook::Hook(char* src, char* dst, int length) {
	if (length < 5) {
		return false;
	}
	else {
		DWORD originalProc;
		VirtualProtect(src, length, PAGE_EXECUTE_READWRITE, &originalProc);
		memset(src, 0x90, length);
		uintptr_t relativeAddy = (uintptr_t)(dst - src - 5);
		*src = (char)0xE9;
		*(uintptr_t*)(src + 1) = (uintptr_t)relativeAddy;
		VirtualProtect(src, length, originalProc, &originalProc);
		return true;
	}
}
char* hook::TrampHook(char* src, char* dst, unsigned int length) {
	if (length < 5) {
		return 0;
	}
	else {
		char* gate = (char*)VirtualAlloc(0, length + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		memcpy(gate, src, length);
		uintptr_t jmpAddy = (uintptr_t)(src - gate - 5);
		*(gate + length) = (char)0xE9;
		*(uintptr_t*)(gate + length + 1) = jmpAddy;
		if (Hook(src, dst, length)) {
			return gate;
		}
		else {
			return nullptr;
		}
	}
}