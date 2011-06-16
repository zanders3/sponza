// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Renderer
//		- Renders effects! (will be replaced eventually by scripting... somehow).
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include "Scene/SceneList.h"
#include "Graphics/RenderTexture.h"
#include "Graphics/DepthTexture.h"
#include "Scene/SceneList.h"
#include <memory>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace graphics
{
	class Shader;
}

namespace scene
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class Renderer;
typedef std::shared_ptr<Renderer> RendererPtr;

class Renderer
{
public:
	struct Pass
	{
		enum Enum
		{
			Normals,
			Lights,
			Color
		};
	};

	Renderer(ID3D10Device* pDevice, graphics::Shader* pShader, SceneListPtr sceneList);

	void Draw();

private:
	void DrawPass(Pass pass);

	Pass			m_pass;
	ID3D10Device*	m_pDevice;

	graphics::Shader*		m_pShader;

	graphics::DepthTexture	m_frameDepth;

	graphics::RenderTexture	m_frameBuffer;
	graphics::RenderTexture	m_normalBuffer;
	graphics::RenderTexture	m_lightBuffer;
	
	SceneListPtr	m_sceneList;
};

// -----------------------------------------------------------------------------

}//namespace scene
