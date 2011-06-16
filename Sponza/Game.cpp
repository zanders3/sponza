// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Game.h"
#include "Content\PackReader.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace scene;
using namespace graphics;
using namespace graphics::model;

namespace game
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------


Game::Game() :
	m_content(".\\Content\\Out", ".\\Content\\Content.pack"),
	m_camera()
{
	DXUTSetCallbackKeyboard(&m_camera.OnKeyboard);
	DXUTSetCallbackMouse(&m_camera.OnMouse, true);
}

// -----------------------------------------------------------------------------

Game::~Game()
{
}

// -----------------------------------------------------------------------------

void Game::LoadContent( ID3D10Device* pd3dDevice, int width, int height )
{
	m_scene = std::make_shared<SceneList>();
	m_renderer = std::make_shared<Renderer>(pd3dDevice, m_content.Get<Shader>("BlankShader.fx"), m_scene);

	const D3DXVECTOR3 zero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scene->Add(m_content.Get<Model>("sponza.obj"), zero);

	Light* light = m_scene->CreateLight();
	light->SetSize(200.0f);
	light->SetPosition(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
	light->SetColor(D3DCOLOR_ARGB(255,255,255,255));
}

// -----------------------------------------------------------------------------

void Game::Render( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime )
{
	m_camera.Update(fElapsedTime);
	Shader::SetView(m_camera.m_View);
	Shader::SetProjection(m_camera.m_Projection);

	m_renderer->Draw();
}

// -----------------------------------------------------------------------------

void Game::Update( double fTime, float fElapsedTime )
{
/*#ifdef _DEBUG
	m_content.ReloadContent();
#endif*/
}

// -----------------------------------------------------------------------------

}//namespace game