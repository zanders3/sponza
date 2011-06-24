// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	SceneNode
//		- Represents a scene graph tree which can be loaded in parts for models.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"
#include "Graphics/Model/Mesh.h"
#include "Graphics/Model/Material.h"
#include <memory>
#include <vector>

#include "Scene/Transform.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{
	class ContentReader;
}

namespace scene
{
	class MeshQueue;

	class Component;
	typedef std::shared_ptr<Component> ComponentPtr;

	class SceneNode;
	typedef std::shared_ptr<SceneNode> SceneNodePtr;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class SceneNode
{
public:
	SceneNode();
	~SceneNode();

	void 
	Load(
		content::ContentReader&				reader, 
		std::vector<graphics::Material>&	materialList
	);

	void
	AddChild(
		SceneNodePtr&	child
	);

	void
	AddMesh(
		graphics::Mesh&	mesh
	);

	Transform&
	GetTransform();

	void
	Cull(
		const D3DXMATRIX&	transform,
		MeshQueue&			meshQueue
	);

private:
	Transform					m_transform;
	D3DXMATRIX					m_rootTransform;

	std::vector<graphics::Mesh>	m_meshList;
	std::vector<SceneNodePtr>	m_children;
};

// -----------------------------------------------------------------------------

}//namespace scene