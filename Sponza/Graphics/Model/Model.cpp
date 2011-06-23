// -----------------------------------------------------------------------------
//	Copyright Alex Parker � 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/Model/Model.h"

#include "Graphics/Shader.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace graphics
{

// -----------------------------------------------------------------------------
// Static Data
// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

Model::Model() :
	m_meshes(),
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
		//m_materials.push_back(Material());
		//m_materials[i].Load(*m_pContent, reader);
	}

	size_t numMeshes = *reader.Read<size_t>();
	m_meshes.assign(numMeshes, Mesh());
	for (size_t i = 0; i<numMeshes; ++i)
	{
		m_meshes[i].Load(reader, m_materials);
	}
}

//----------------------------------------------------------------------------------------

}//namespace graphics