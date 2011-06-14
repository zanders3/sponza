// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
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
namespace model
{

// -----------------------------------------------------------------------------
// Static Data
// -----------------------------------------------------------------------------

D3D10_INPUT_ELEMENT_DESC Model::s_layoutDesc[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, 48, D3D10_INPUT_PER_VERTEX_DATA, 0 }
};

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
	m_materials.assign(numMaterials, Material());
	for (size_t i = 0; i<numMaterials; ++i)
	{
		m_materials[i].Load(reader, m_pContent);
	}

	size_t numMeshes = *reader.Read<size_t>();
	m_meshes.assign(numMeshes, Mesh());
	for (size_t i = 0; i<numMeshes; ++i)
	{
		m_meshes[i].Load(reader, m_materials);
	}
}

// -----------------------------------------------------------------------------

void
Model::Draw()
{
	static InputLayout inputLayout(reinterpret_cast<D3D10_INPUT_ELEMENT_DESC*>(&s_layoutDesc), 5);
	inputLayout.Bind();

	m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (auto iter = m_meshes.begin(); iter != m_meshes.end(); ++iter)
	{
		iter->Draw();
	}
}

//----------------------------------------------------------------------------------------

}//namespace model
}//namespace graphics