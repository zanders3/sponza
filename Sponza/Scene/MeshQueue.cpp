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

MaterialList::MaterialList(
	graphics::Material& material
) : material(material)
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
	auto iter = std::upper_bound(
		m_queue.begin(), 
		m_queue.end(), 
		mesh->GetMaterial(), 
		[](const graphics::Material& material, const std::unique_ptr<MaterialList>& list)
		{
			return material < list->material;
		});

	if (iter == m_queue.end())
	{
		m_queue.push_back(std::unique_ptr<MaterialList>( new MaterialList( mesh->GetMaterial() ) ));
		iter = m_queue.end() - 1;
	}

	(*iter)->meshList.push_back(MeshQueueItem(world, mesh));
}

//---------------------------------------------------------------------------------------

void
MeshQueue::Clear()
{
	for (auto iter = m_queue.begin(); iter != m_queue.end(); ++iter)
		(*iter)->meshList.clear();
}

//---------------------------------------------------------------------------------------

void
MeshQueue::Draw()
{
	for (auto materialIter = m_queue.begin(); materialIter != m_queue.end(); ++materialIter)
	{
		MaterialList& list = **materialIter;
		if (!list.meshList.empty())
		{
			list.material.Bind();

			//TODO: more sorting to support mesh instancing!
			for (auto meshIter = list.meshList.begin(); meshIter != list.meshList.end(); ++meshIter)
			{
				list.material.SetWorld(*(meshIter->world));
				meshIter->mesh->Draw();
			}
		}
	}
}

//---------------------------------------------------------------------------------------

}//namespace scene