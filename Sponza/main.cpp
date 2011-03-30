// Sponza.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Game.h"

Game* game;

bool CALLBACK IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType,
                                       DXGI_FORMAT BufferFormat, bool bWindowed, void* pUserContext )
{
    return true;
}

HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBufferSurfaceDesc,
                                      void* pUserContext )
{
	game->LoadContent(pd3dDevice, pBufferSurfaceDesc->Width, pBufferSurfaceDesc->Height);

	return S_OK;
}

void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	game->Render( pd3dDevice, fTime, fElapsedTime );
}

void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	game->Update( fTime, fElapsedTime );
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	DXUTSetCallbackD3D10DeviceAcceptable( IsD3D10DeviceAcceptable );
	DXUTSetCallbackD3D10DeviceCreated( OnD3D10CreateDevice );
	DXUTSetCallbackD3D10FrameRender( OnD3D10FrameRender );
	DXUTSetCallbackFrameMove( OnFrameMove );

	try
	{
		game = new Game();
	
		DXUTInit();
		DXUTCreateWindow(L"Sponza");
		DXUTCreateDevice( true, 640, 480 );
		DXUTMainLoop();

		delete game;
	}
	catch (std::exception excp)
	{
		OutputDebugStringA(excp.what());
		MessageBoxA(DXUTGetHWND(), excp.what(), "Exception in Sponza", 0);
	}

	return DXUTGetExitCode();
}