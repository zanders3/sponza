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
namespace model
{

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

	void Create(
		Vertex*			pVertex, 
		size_t			numVertices, 
		size_t*			pIndices, 
		size_t			numIndices,
		ID3D10Device*	pDevice
	);

	void Load(
		content::ContentReader&	reader, 
		std::vector<Material>&	materials
	);

	void Draw();

private:
	ID3D10Buffer* mVertexBuffer;
	ID3D10Buffer* mIndexBuffer;
	Material*	  mMaterial;
	size_t		  mNumIndices;
};

// -----------------------------------------------------------------------------

}//namespace model
}//namespace graphics