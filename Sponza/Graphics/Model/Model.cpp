// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/Model/Model.h"
#include "Scene/SceneNode.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace graphics
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

Model::Model() :
	m_materials()
{
}

// -----------------------------------------------------------------------------

Model::~Model()
{
}

// -----------------------------------------------------------------------------

void 
Model::Load(
	content::ContentReader& reader
)
{
	size_t numMaterials = *reader.Read<size_t>();
	m_materials.reserve(numMaterials);
	for (size_t i = 0; i<numMaterials; ++i)
	{
		m_materials.push_back(Material());
		m_materials[i].Load(*m_pContent, reader);
	}

	m_rootNode = std::make_shared<scene::SceneNode>();

	size_t numMeshes = *reader.Read<size_t>();
	for (size_t i = 0; i<numMeshes; ++i)
	{
		Mesh mesh;
		mesh.Load(reader, m_materials);
		m_rootNode->AddMesh(mesh);
	}
}

//----------------------------------------------------------------------------------------

scene::SceneNodePtr&
Model::GetModelRoot()
{
	return m_rootNode;
}

//----------------------------------------------------------------------------------------

}//namespace graphics