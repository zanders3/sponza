// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "DXUT.h"
#include "DXUTmisc.h"

#if _DEBUG

class _PIXEvent
{
public:
	_PIXEvent(LPCWSTR wszName)
	{
		static const D3DCOLOR col = D3DCOLOR_RGBA(255, 255, 255, 255);
		D3DPERF_BeginEvent(col, wszName);
	}

	~_PIXEvent()
	{
		D3DPERF_EndEvent();
	}
};

#define PIXEvent(name) _PIXEvent pixScopeEvent(name)

#else

#define PIXEvent(name) __noop

#endif

ID3D10Device* GetDevice();

typedef unsigned long long u64;
typedef unsigned int u32;

#if defined(DEBUG) || defined(_DEBUG)

#define _assert(condition) { if (!(condition)) { DXUTTrace(__FILE__, __LINE__, 1, L#condition , true); } }

#else

#define _assert(condition, expr) __noop

#endif