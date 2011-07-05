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
#include "Scene/SceneNode.h"
#include "Scene/MeshQueue.h"
#include "Script/ScriptEngine.h"
#include "Script/Script.h"
#include "Graphics/Model/Model.h"
#include "Scene/Renderer.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace game
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------


Game::Game(
) : m_content(".\\..\\Content\\Out", ".\\..\\Content\\Content.pack"),
	m_camera(new Camera()),
	m_sceneRoot(new scene::SceneNode()),
	m_meshQueue(new scene::MeshQueue()),
	m_scriptEngine(new script::ScriptEngine()),
	m_renderer(new scene::Renderer(*m_scriptEngine, *m_meshQueue))
{
	DXUTSetCallbackKeyboard(&m_camera->OnKeyboard);
	DXUTSetCallbackMouse(&m_camera->OnMouse, true);

	D3DXMatrixIdentity(&m_identity);
}

// -----------------------------------------------------------------------------

Game::~Game()
{
}

// -----------------------------------------------------------------------------

void Game::LoadContent( ID3D10Device* pd3dDevice, int width, int height )
{
	const D3DXVECTOR3 zero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_sceneRoot->AddChild(m_content.GetContent<graphics::Model>("sponza.obj")->GetModelRoot());

	/*script->Invoke<void ()>("void EmptyTest()")();
	int returned = script->Invoke<int ()>("int RetInt()")();
	double ret = script->Invoke<double (double)>("double One(double a)")(5.0);
	script->Invoke<void (int, int)>("void MultiValue(int a, int b)")(5, 10);*/

	/*Light* light = m_scene->CreateLight();
	light->SetSize(200.0f);
	light->SetPosition(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
	light->SetColor(D3DCOLOR_ARGB(255,255,255,255));*/

	m_renderer->LoadContent(m_content);

	graphics::GlobalShaderParams::SetValue("Projection", &m_camera->m_Projection);

	D3DXVECTOR2 screenSize(640.0f, 480.0f);
	graphics::GlobalShaderParams::SetValue<D3DXVECTOR2&>("ScreenSize", screenSize);
}

// -----------------------------------------------------------------------------

void Game::Render( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime )
{
	graphics::GlobalShaderParams::SetValue("View", &m_camera->m_View);
	graphics::GlobalShaderParams::Apply();
	
/*	float clearColor[] = { 0.0f, 0.2f, 1.0f, 1.0f };
	GetDevice()->ClearRenderTargetView(DXUTGetD3D10RenderTargetView(), clearColor);
	GetDevice()->ClearDepthStencilView(DXUTGetD3D10DepthStencilView(), D3D10_CLEAR_DEPTH, 1.0f, 0);*/

	m_renderer->Draw();
}

// -----------------------------------------------------------------------------

void Game::Update( double fTime, float fElapsedTime )
{
	m_content.Update();

	m_camera->Update(fElapsedTime);

	m_meshQueue->Clear();
	m_sceneRoot->Cull(m_identity, *m_meshQueue);
}

// -----------------------------------------------------------------------------

}//namespace game