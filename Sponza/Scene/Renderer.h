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

namespace script
{
	class ScriptEngine;
}

namespace graphics
{
	class RenderTexture;
}

namespace scene
{
	class MeshQueue;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class Renderer
{
public:
	Renderer(script::ScriptEngine& engine, MeshQueue& meshQueue);
	~Renderer();

	void
	Draw();

private:
	MeshQueue&												m_meshQueue;
	std::vector<std::unique_ptr<graphics::RenderTexture>>	m_renderTextureList;
};

// -----------------------------------------------------------------------------

}//namespace scene
