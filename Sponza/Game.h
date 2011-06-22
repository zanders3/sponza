// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Game
//		- Main Game class that wraps DXUT.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include "resource.h"
#include "Content\ContentManager.h"
#include "Graphics\Model\Model.h"
#include "Graphics\Shader.h"
#include "Graphics\Camera.h"
#include "Scene\Renderer.h"
#include "Scene\SceneList.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace game
{

class Game
{
public:
	Game();
	~Game();

	void LoadContent( ID3D10Device* pd3dDevice, int width, int height );
	void Render( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime );
	void Update( double fTime, float fElapsedTime ); 

private:
	content::ContentManager m_content;
	Camera					m_camera;

	scene::SceneListPtr		m_scene;
	scene::RendererPtr		m_renderer;
	D3DXMATRIX				m_world;
};

// -----------------------------------------------------------------------------

}//namespace game