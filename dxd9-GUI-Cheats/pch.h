// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
struct Vector2 { int x, y; };

#include "framework.h"
#include <iostream>
#include <String>
#include <typeindex>
#include <vector>
#include <Windows.h>
#include <Xinput.h>
#include <TlHelp32.h>
//#include <D3DX10core.h>
//#include <d3d10.h>
//#include <d3dx10.h>
#include <d3dx9.h>
#include <d3d9.h>

#include <d3d.h>
#include "mem.h"
#include "hook.h"
#include "dxd9.h"
#include "proc.h"
#include "Menu.h"
#include "Gamepad.h"

//#pragma comment(lib, "d3dx10.lib")
//#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "Xinput9_1_0.lib")

#undef UNICODE

#endif //PCH_H
