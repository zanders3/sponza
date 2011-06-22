// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Game.h"
#include "Content/PackReader.h"
#include "Graphics/Shader/ShaderParams.h"

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
	m_content(".\\..\\Content\\Out", ".\\..\\Content\\Content.pack"),
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
	m_renderer = std::make_shared<Renderer>(pd3dDevice, m_content.GetContent<Shader>("BlankShader.fx"), m_scene);

	const D3DXVECTOR3 zero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scene->Add(m_content.GetContent<Model>("sponza.obj"), zero);

	Light* light = m_scene->CreateLight();
	light->SetSize(200.0f);
	light->SetPosition(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
	light->SetColor(D3DCOLOR_ARGB(255,255,255,255));

	D3DXMatrixIdentity(&m_world);
	graphics::GlobalShaderParams::SetValue("Projection", &m_camera.m_Projection);

	D3DXVECTOR2 screenSize(640.0f, 480.0f);
	graphics::GlobalShaderParams::SetValue<D3DXVECTOR2&>("ScreenSize", screenSize);
}

// -----------------------------------------------------------------------------

void Game::Render( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime )
{
	m_camera.Update(fElapsedTime);

	graphics::GlobalShaderParams::SetValue("View", &m_camera.m_View);
	graphics::GlobalShaderParams::SetValue("World", &m_world);
	graphics::GlobalShaderParams::Apply();

	m_renderer->Draw();
}

// -----------------------------------------------------------------------------

void Game::Update( double fTime, float fElapsedTime )
{
	m_content.Update();
}

// -----------------------------------------------------------------------------

}//namespace game