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
#include "Content/ContentItem.h"
#include <memory>
#include <vector>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace graphics
{
	class Mesh;
	class Shader;
}

namespace scene
{
// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

struct MeshQueueItem
{
	MeshQueueItem(
		D3DXMATRIX*		world,
		graphics::Mesh*	mesh
	);

	D3DXMATRIX*		world;
	graphics::Mesh* mesh;
};

// -----------------------------------------------------------------------------

struct ShaderList
{
	ShaderList(
		graphics::Shader* material
	);

	graphics::Shader*			shader;
	std::vector<MeshQueueItem>	meshList;
};

// -----------------------------------------------------------------------------

class MeshQueue
{
public:
	MeshQueue();
	~MeshQueue();

	void
	Push(
		D3DXMATRIX*		world,
		graphics::Mesh*	mesh
	);

	void
	Clear();

	void
	Draw();

private:
	std::vector<ShaderList> m_queue;
};

// -----------------------------------------------------------------------------

}//namespace scene