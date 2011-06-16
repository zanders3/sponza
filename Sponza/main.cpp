// main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Game.h"

game::Game* pGame;
ID3D10Device* pDevice;

bool CALLBACK IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType,
                                       DXGI_FORMAT BufferFormat, bool bWindowed, void* pUserContext )
{
    return true;
}

HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBufferSurfaceDesc,
                                      void* pUserContext )
{
	pDevice = pd3dDevice;
	pGame->LoadContent(pd3dDevice, pBufferSurfaceDesc->Width, pBufferSurfaceDesc->Height);

	return S_OK;
}

void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	pGame->Render( pd3dDevice, fTime, fElapsedTime );
}

void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
	pGame->Update( fTime, fElapsedTime );
}

HRESULT CALLBACK OnD3D10Resized( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	return S_OK;
}

ID3D10Device* GetDevice()
{
	return pDevice;
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
	DXUTSetCallbackD3D10SwapChainResized( OnD3D10Resized );
	DXUTSetCallbackFrameMove( OnFrameMove );

	try
	{
		pGame = new game::Game();
	
		DXUTInit();
		DXUTCreateWindow(L"Sponza");
		DXUTCreateDevice( true, 640, 480 );
		DXUTMainLoop();

		delete pGame;
	}
	catch (std::exception excp)
	{
		OutputDebugStringA(excp.what());
		MessageBoxA(DXUTGetHWND(), excp.what(), "Exception in Sponza", 0);
	}

	return DXUTGetExitCode();
}