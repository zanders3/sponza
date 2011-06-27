// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Scene/MeshQueue.h"
#include "Graphics/Model/Mesh.h"
#include "Graphics/Model/Material.h"
#include "Graphics/Shader.h"
#include <algorithm>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace scene
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

MeshQueueItem::MeshQueueItem(
	D3DXMATRIX*		world,
	graphics::Mesh*	mesh
) : world(world),
	mesh(mesh)
{
}

// -----------------------------------------------------------------------------

ShaderList::ShaderList(
	graphics::Shader* shader
) : shader(shader)
{
}

// -----------------------------------------------------------------------------

MeshQueue::MeshQueue(
) : m_queue()
{
}

//---------------------------------------------------------------------------------------

MeshQueue::~MeshQueue()
{
}

//---------------------------------------------------------------------------------------

void
MeshQueue::Push(
	D3DXMATRIX*		world,
	graphics::Mesh*	mesh
)
{
	graphics::Shader* shader = mesh->GetMaterial().GetShader();

	for (auto iter = m_queue.begin(); iter != m_queue.end(); ++iter)
	{
		if (*iter->shader == *shader)
		{
			iter->meshList.push_back(MeshQueueItem(world, mesh));
			return;
		}
	}

	m_queue.push_back(ShaderList(shader));
	m_queue.back().meshList.push_back(MeshQueueItem(world, mesh));
}

//---------------------------------------------------------------------------------------

void
MeshQueue::Clear()
{
	for (auto iter = m_queue.begin(); iter != m_queue.end(); ++iter)
		iter->meshList.clear();
}

//---------------------------------------------------------------------------------------

void
MeshQueue::Draw()
{
	std::sort(
		m_queue.begin(), 
		m_queue.end(), 
		[](const ShaderList& lhs, const ShaderList& rhs)
		{
			return *lhs.shader < *rhs.shader;
		});

	for (auto shaderIter = m_queue.begin(); shaderIter != m_queue.end(); ++shaderIter)
	{
		ShaderList& list = *shaderIter;
		if (!list.meshList.empty())
		{
			for (auto meshIter = list.meshList.begin(); meshIter != list.meshList.end(); ++meshIter)
			{
				meshIter->mesh->GetMaterial().SetWorld(*(meshIter->world));
				meshIter->mesh->Draw();
			}
		}
	}
}

//---------------------------------------------------------------------------------------

}//namespace scene