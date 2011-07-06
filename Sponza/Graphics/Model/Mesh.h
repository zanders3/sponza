// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Model
//		- Loads and handles models and associated materials.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include <vector>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{
	class ContentReader;
}

namespace graphics
{
class InputLayout;

class Material;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

struct Vertex
{
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mNormal;
	D3DXVECTOR3 mTangent;
	D3DXVECTOR3 mBiNormal;
	D3DXVECTOR2 mTexCoord;
};

// -----------------------------------------------------------------------------

class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh(
		Mesh&& other
	);

	Material&
	GetMaterial();

	void Create(
		D3D10_PRIMITIVE_TOPOLOGY topology,
		Vertex*			pVertex,
		size_t			numVertices, 
		size_t*			pIndices, 
		size_t			numIndices
	);

	void Load(
		content::ContentReader&	reader, 
		std::vector<Material>&	materials
	);

	void Draw(u32 passHash = 0);

private:
	Mesh(const Mesh& other) {}

	D3D10_PRIMITIVE_TOPOLOGY	m_topology;
	ID3D10Buffer*				mVertexBuffer;
	ID3D10Buffer*				mIndexBuffer;
	Material*					mMaterial;
	size_t						mNumIndices;
};

// -----------------------------------------------------------------------------

}//namespace graphics