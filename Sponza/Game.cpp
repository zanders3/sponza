#include "stdafx.h"
#include "Game.h"

#include "Content/ContentIDs.h"
#include "Content/ContentLoader.h"

Game::Game() :
	m_content(".\\Content"),
	m_camera()
{
	DXUTSetCallbackKeyboard(&m_camera.OnKeyboard);
	DXUTSetCallbackMouse(&m_camera.OnMouse, true);
}

Game::~Game()
{
}

void Game::LoadContent( ID3D10Device* pd3dDevice, int width, int height )
{
	m_content.SetDevice(pd3dDevice);

	m_scene = std::make_shared<SceneList>();
	m_renderer = std::make_shared<Renderer>(pd3dDevice, m_content.Get<Shader>(ContentID::BLANKSHADER), m_scene);

	const D3DXVECTOR3 zero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scene->Add(m_content.Get<Model>(ContentID::SPONZA), zero);
}

void Game::Render( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime )
{
	m_camera.Update(fElapsedTime);
	Shader::SetView(m_camera.m_View);
	Shader::SetProjection(m_camera.m_Projection);

	
	/*float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
    pd3dDevice->ClearRenderTargetView( pRTV, ClearColor );*/

	//ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
    //pd3dDevice->ClearDepthStencilView( pDSV, D3D10_CLEAR_DEPTH, 1.0, 0 );

	m_renderer->Draw();

	//D3DXMATRIX identity;
	//D3DXMatrixIdentity(&identity);
	//m_content.Get<Shader>(ContentID::BLANKSHADER)->SetWorld(identity);

	//m_content.Get<Shader>(ContentID::BLANKSHADER)->Bind(0);
	//m_model->Draw();
	//m_scene->Draw(m_content.Get<Shader>(ContentID::BLANKSHADER));
}

void Game::Update( double fTime, float fElapsedTime )
{
#ifdef _DEBUG
	m_content.ReloadContent();
#endif


}