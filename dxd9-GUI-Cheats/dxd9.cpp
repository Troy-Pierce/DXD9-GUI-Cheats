#include "pch.h"

HWND window;

BOOL CALLBACK enumWind(HWND handle, LPARAM lp) {
	DWORD procId;
	GetWindowThreadProcessId(handle, &procId);
	if (GetCurrentProcessId() != procId) {
		return TRUE;
	}
	window = handle;
	return FALSE;
}

HWND dxd9::GetWindow() {
	window = NULL;
	EnumWindows(enumWind, NULL);
	return window;
}

bool dxd9::GetDevice(void** pTable, size_t size) {
	if (!pTable) {
		return false;
	}
	IDirect3D9* dx3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!dx3D) {
		return false;
	}
	IDirect3DDevice9* pDDevice = nullptr;
	D3DPRESENT_PARAMETERS dxParam = {};
	dxParam.Windowed = false;
	dxParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dxParam.hDeviceWindow = GetWindow();
	HRESULT dxDDeviceResult = dx3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, dxParam.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dxParam, &pDDevice);
	if (dxDDeviceResult != S_OK) {
		dxParam.Windowed = !dxParam.Windowed;
		dxDDeviceResult = dx3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, dxParam.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &dxParam, &pDDevice);
		if (dxDDeviceResult != S_OK) {
			dx3D->Release();
			return false;
		}
	}
	memcpy(pTable, *(void***)(pDDevice), size);
	pDDevice->Release();
	dx3D->Release();
	return true;

}

void dxd9::DrawBox(int x, int y, int width, int height, int red, int green, int blue) {
	D3DRECT dxRect = { x, y, x + width, y + height };
	dxd9::pDevice->Clear (1, &dxRect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(NULL, red, green, blue), 0, 0);
}
void dxd9::DrawBorder(int x, int y, int width, int height, int size, int red, int green, int blue) {
	dxd9::DrawBox(x, y - size, width, size, red, green, blue);
	dxd9::DrawBox(x, y + height - size, width + size, size, red, green, blue);
	dxd9::DrawBox(x, y - size, size, height + size, red, green, blue);
	dxd9::DrawBox(x + width, y - size, size, height + size, red, green, blue);
}
int dxd9::DrawString(const char* string, int x, int y, int red, int green, int blue) {
	RECT Font;
	Font.bottom = 0;
	Font.left = x;
	Font.top = y;
	Font.right = 0;
	return dxd9::dxdFont->DrawTextA(NULL, string, strlen(string), &Font, DT_NOCLIP, D3DCOLOR_ARGB(255, red, green, blue));
}