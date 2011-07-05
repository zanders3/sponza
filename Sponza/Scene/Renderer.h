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
#include <memory>
#include <vector>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{
	class ContentManager;
}

namespace script
{
	class ScriptEngine;
	class Script;
}

namespace graphics
{
	class RenderTexture;
	class DepthTexture;
}

namespace scene
{
	class MeshQueue;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class Renderer
{
	friend class RenderTargetHandle;
public:
	Renderer(script::ScriptEngine& engine, MeshQueue& meshQueue);
	~Renderer();

	void
	Draw();

	graphics::RenderTexture&
	GetFramebuffer()
	{
		return *m_frameBuffer;
	}

	graphics::RenderTexture&
	CreateRenderTexture(int width, int height);

	void
	DeleteRenderTexture(graphics::RenderTexture* texture);

	void
	LoadContent(
		content::ContentManager& contentManager
	);

	void
	DrawPass();

private:
	MeshQueue&												m_meshQueue;
	script::Script*											m_renderScript;

	std::unique_ptr<graphics::DepthTexture>					m_depthBuffer;
	std::unique_ptr<graphics::RenderTexture>				m_frameBuffer;
	std::vector<std::unique_ptr<graphics::RenderTexture>>	m_renderTextureList;
};

// -----------------------------------------------------------------------------

}//namespace scene
