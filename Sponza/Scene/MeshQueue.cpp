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
	graphics::Material& material = mesh->GetMaterial();

	auto find = std::lower_bound(
		m_queue.begin(),
		m_queue.end(), 
		mesh->GetMaterial(), 
		[](const std::unique_ptr<MaterialList>& list, const graphics::Material& material)
		{
			return material < list->material;
		});

	if (find != m_queue.end() && material == (*find)->material)
	{
		(*find)->meshList.push_back(MeshQueueItem(world, mesh));
	}
	else
	{
		m_queue.insert(find, std::unique_ptr<MaterialList>( new MaterialList( mesh->GetMaterial() ) ));
		m_queue.back()->meshList.push_back(MeshQueueItem(world, mesh));
	}
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