// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/Model/Mesh.h"
#include "Graphics/InputLayout.h"

#include "Graphics/Model/Material.h"
#include "Content/ContentReader.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace graphics
{
namespace model
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

Mesh::Mesh() :
	mVertexBuffer(nullptr),
	mIndexBuffer(nullptr),
	mMaterial(nullptr)
{
}

// -----------------------------------------------------------------------------

Mesh::~Mesh()
{
	SAFE_RELEASE(mVertexBuffer);
	SAFE_RELEASE(mIndexBuffer);
}

// -----------------------------------------------------------------------------

void 
Mesh::Load(
	content::ContentReader&	reader, 
	std::vector<Material>&	materials
)
{
	size_t   mMaterialID = *reader.Read<size_t>();
	mMaterial = &materials[mMaterialID];

	size_t	 mNumVertices = *reader.Read<size_t>();
	Vertex * mVertices = reader.Read<Vertex>(mNumVertices);

	size_t	 mNumIndices = *reader.Read<size_t>();
	size_t * mIndices = reader.Read<size_t>(mNumIndices);

	Create(mVertices, mNumVertices, mIndices, mNumIndices, GetDevice());
}

// -----------------------------------------------------------------------------

void 
Mesh::Create(
	Vertex* pVertex, 
	size_t numVertices, 
	size_t* pIndices, 
	size_t numIndices, 
	ID3D10Device* pDevice)
{
	HRESULT hr;

	D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( Vertex ) * numVertices;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = pVertex;
	V(pDevice->CreateBuffer( &bd, &InitData, &mVertexBuffer));

	bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( size_t ) * numIndices;
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    InitData.pSysMem = pIndices;
    V(pDevice->CreateBuffer( &bd, &InitData, &mIndexBuffer));

	mNumIndices = numIndices;
}

// -----------------------------------------------------------------------------

void 
Mesh::Draw(
	InputLayout& layout)
{
	mMaterial->Bind();
	layout.Bind();

	GetDevice()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	GetDevice()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	GetDevice()->DrawIndexed(mNumIndices, 0, 0);
}

// -----------------------------------------------------------------------------

}//namespace model
}//namespace graphics