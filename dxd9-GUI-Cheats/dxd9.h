#pragma once
#include "pch.h"

namespace dxd9 {
	typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);
	HWND GetWindow();
	void DrawBox(int x, int y, int width, int height, int red, int green, int blue);
	void DrawBorder(int x, int y, int width, int height, int size, int red, int green, int blue);
	int DrawString(const char* string, int x, int y, int red, int green, int blue);
	bool GetDevice(void** pTable, size_t size);
	extern ID3DXFont* dxdFont;
	extern void* d3d9Device[119];
	extern BYTE EndScene[7];
	extern LPDIRECT3DDEVICE9 pDevice;
	extern tEndScene originalEndScene;
}