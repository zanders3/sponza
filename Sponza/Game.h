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
#include "Graphics\Camera.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace scene
{
	class SceneNode;
	class MeshQueue;
}

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
	content::ContentManager				m_content;
	std::unique_ptr<Camera>				m_camera;
	std::unique_ptr<scene::SceneNode>	m_sceneRoot;
	std::unique_ptr<scene::MeshQueue>	m_meshQueue;

	D3DXMATRIX							m_identity;
};

// -----------------------------------------------------------------------------

}//namespace game