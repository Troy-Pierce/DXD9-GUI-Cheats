#pragma once
namespace hook {
	bool Hook(char* src, char*, int length);
	char* TrampHook(char* src, char* dst, unsigned int length);
	
}

