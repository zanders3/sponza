#include "stdafx.h"
#include "Game.h"

#include "Content/ContentIDs.h"
#include "Content/ContentLoader.h"

Game::Game() :
	m_content("..\\Content\\Out"),
	m_camera()
{
	DXUTSetCallbackKeyboard(&m_camera.OnKeyboard);
	DXUTSetCallbackMouse(&m_camera.OnMouse);
}

Game::~Game()
{
}

void Game::LoadContent( ID3D10Device* pd3dDevice, int width, int height )
{
	m_content.SetDevice(pd3dDevice);

	m_model = m_content.Get<Model>(ContentID::SPONZA_ATRIUM);
	m_shader = m_content.Get<Shader>(ContentID::BLANKSHADER);
}

void Game::Render( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime )
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();
    pd3dDevice->ClearRenderTargetView( pRTV, ClearColor );

	ID3D10DepthStencilView* pDSV = DXUTGetD3D10DepthStencilView();
    pd3dDevice->ClearDepthStencilView( pDSV, D3D10_CLEAR_DEPTH, 1.0, 0 );

	m_camera.Update(fElapsedTime);
	Shader::SetView(m_camera.m_View);
	Shader::SetProjection(m_camera.m_Projection);

	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	m_shader->SetWorld(identity);

	m_shader->Bind(0);
	m_model->Draw();
}

void Game::Update( double fTime, float fElapsedTime )
{
	m_content.Update();
}