// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/Model/Mesh.h"
#include "Graphics/InputLayout.h"
#include "Content/ContentReader.h"
#include "Graphics/Model/Material.h"

// -----------------------------------------------------------------------------
// Static Data
// -----------------------------------------------------------------------------

namespace
{
	D3D10_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, 48, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D10_PRIMITIVE_TOPOLOGY currentTopology = D3D10_PRIMITIVE_TOPOLOGY_UNDEFINED;
}

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace graphics
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

Mesh::Mesh(
	Mesh&& other
) : m_topology(other.m_topology),
	mVertexBuffer(other.mVertexBuffer),
	mIndexBuffer(other.mIndexBuffer),
	mMaterial(other.mMaterial),
	mNumIndices(other.mNumIndices)
{
	other.mVertexBuffer = nullptr;
	other.mIndexBuffer = nullptr;
	other.mMaterial = nullptr;
}

// -----------------------------------------------------------------------------

Material&
Mesh::GetMaterial()
{
	return *mMaterial;
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

	Create(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, mVertices, mNumVertices, mIndices, mNumIndices);
}

// -----------------------------------------------------------------------------

void 
Mesh::Create(
	D3D10_PRIMITIVE_TOPOLOGY topology,
	Vertex* pVertex, 
	size_t numVertices, 
	size_t* pIndices, 
	size_t numIndices)
{
	m_topology = topology;

	HRESULT hr;

	D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( Vertex ) * numVertices;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = pVertex;
	V(GetDevice()->CreateBuffer( &bd, &InitData, &mVertexBuffer));

	bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( size_t ) * numIndices;
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    InitData.pSysMem = pIndices;
    V(GetDevice()->CreateBuffer( &bd, &InitData, &mIndexBuffer));

	mNumIndices = numIndices;
}

// -----------------------------------------------------------------------------

void 
Mesh::Draw()
{
	if (mMaterial)
	{
		mMaterial->Bind();
	}

	if (currentTopology != m_topology)
	{
		currentTopology = m_topology;
		GetDevice()->IASetPrimitiveTopology(m_topology);
	}

	static InputLayout layout(reinterpret_cast<D3D10_INPUT_ELEMENT_DESC*>(&layoutDesc), 5);
	layout.Bind();

	GetDevice()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	GetDevice()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	GetDevice()->DrawIndexed(mNumIndices, 0, 0);
}

// -----------------------------------------------------------------------------

}//namespace graphics