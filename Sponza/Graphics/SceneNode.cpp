// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Scene/SceneNode.h"
#include "Scene/MeshQueue.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace scene
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

SceneNode::SceneNode()
{
}

// -----------------------------------------------------------------------------

SceneNode::~SceneNode()
{
}

// -----------------------------------------------------------------------------

void 
SceneNode::Load(
	content::ContentReader&				reader, 
	std::vector<graphics::Material>&	materialList
)
{
	//TODO later.
}

// -----------------------------------------------------------------------------

void
SceneNode::AddChild(
	SceneNodePtr&	child
)
{
	m_children.push_back(child);
}

// -----------------------------------------------------------------------------

void
SceneNode::AddMesh(
	graphics::Mesh&	mesh
)
{
	m_meshList.push_back(std::move(mesh));
}

// -----------------------------------------------------------------------------

Transform&
SceneNode::GetTransform()
{
	return m_transform;
}

// -----------------------------------------------------------------------------

void
SceneNode::Cull(
	const D3DXMATRIX&	transform,
	MeshQueue&			meshQueue
)
{
	D3DXMatrixMultiply(&m_rootTransform, &transform, &m_transform.GetMatrix());

	for (auto iter = m_meshList.begin(); iter != m_meshList.end(); ++iter)
	{
		meshQueue.Push(&m_rootTransform, &(*iter));
	}

	for (auto iter = m_children.begin(); iter != m_children.end(); ++iter)
	{
		(*iter)->Cull(m_rootTransform, meshQueue);
	}
}

//---------------------------------------------------------------------------------------

}//namespace scene