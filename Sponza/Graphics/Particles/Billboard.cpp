// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/Particles/Billboard.h"
#include "Graphics/InputLayout.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace graphics
{
namespace particles
{

// -----------------------------------------------------------------------------
// Static Data
// -----------------------------------------------------------------------------
D3D10_INPUT_ELEMENT_DESC Billboard::s_layout[] =
{
	{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	0,	D3D10_INPUT_PER_VERTEX_DATA,	0 },
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	1,	0,	D3D10_INPUT_PER_INSTANCE_DATA,	1 },
	{ "COLOR",		0,	DXGI_FORMAT_R8G8B8A8_UINT,		1,	12,	D3D10_INPUT_PER_INSTANCE_DATA,	1 },
	{ "NORMAL",		0,	DXGI_FORMAT_R32_FLOAT,			1,	16,	D3D10_INPUT_PER_INSTANCE_DATA,	1 }
};

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

Billboard::Billboard(
	bool useCircle, 
	size_t maxCount
) : m_maxCount(maxCount)
{
	if (useCircle)
	{
		static const int numVerts = 32 + 1;
		BillboardVertex vertices[numVerts];

		static const int numIndices = (numVerts-1)*3;
		size_t indices[numIndices];

		static const float interval = (3.1417f*2.0f) / (numVerts - 1);
		vertices[0].x = 0.0f;
		vertices[0].y = 0.0f;

		float d = 0.0f;
		for (int i = 1; i<numVerts; ++i, d+=interval)
		{
			vertices[i].x = sin(d) * 0.5f;
			vertices[i].y = cos(d) * 0.5f;
		}

		int ind = 0;
		for (int i = 2; i<numVerts; ++i)
		{
			indices[ind++] = 0;
			indices[ind++] = i - 1;
			indices[ind++] = i;
		}
		indices[ind++] = 0;
		indices[ind++] = numVerts - 1;
		indices[ind++] = 1;

		Create((BillboardVertex*)&vertices, numVerts, (size_t*)&indices, numIndices);
	}
	else
	{
		//Create Billboard Vertices
		BillboardVertex vertices[] =
		{
			{ -0.5f, -0.5f },
			{ -0.5f,  0.5f },
			{  0.5f,  0.5f },
			{  0.5f, -0.5f }
		};

		//Create Index Buffer
		size_t indices[] =
		{
			0, 1, 3,
			1, 2, 3
		};
		Create((BillboardVertex*)&vertices, 4, (size_t*)&indices, 6);
	}

	//Create the Instance Buffer
	HRESULT hr;
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( BillboardInst ) * maxCount;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;
	V(GetDevice()->CreateBuffer( &bd, NULL, &m_pInstBuffer));
}

//----------------------------------------------------------------------------------------

Billboard::~Billboard()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pInstBuffer);
}

//----------------------------------------------------------------------------------------

void Billboard::Create(BillboardVertex* vertices, size_t vCount, size_t* indices, size_t iCount)
{
	HRESULT hr;
	D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( BillboardVertex ) * vCount;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = vertices;
	V(GetDevice()->CreateBuffer( &bd, &InitData, &m_pVertexBuffer));

	bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( size_t ) * iCount;
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    InitData.pSysMem = indices;
    V(GetDevice()->CreateBuffer( &bd, &InitData, &m_pIndexBuffer));

	m_indexCount = iCount;
}

//----------------------------------------------------------------------------------------

void Billboard::Draw()
{
	static InputLayout layout((D3D10_INPUT_ELEMENT_DESC*)&s_layout, 4);
	layout.Bind();

	static const UINT strides[] = { sizeof(BillboardVertex), sizeof(BillboardInst) };
	static const UINT offsets[] = { 0, 0 };

	GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDevice()->IASetVertexBuffers(0, 2, &m_pVertexBuffer, (UINT*)&strides, (UINT*)&offsets);
	GetDevice()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	GetDevice()->DrawIndexedInstanced(m_indexCount, m_instanceCount, 0, 0, 0);
}

//----------------------------------------------------------------------------------------

void Billboard::SetBillboards(BillboardInst* pInstances, size_t count)
{
	m_instanceCount = min(count, m_maxCount);

	void* pBillboards;
	HRESULT hr;
	V(m_pInstBuffer->Map(D3D10_MAP_WRITE_DISCARD, NULL, &pBillboards));

	memcpy(pBillboards, pInstances, m_instanceCount * sizeof(BillboardInst));

	m_pInstBuffer->Unmap();
}

//----------------------------------------------------------------------------------------

}//namespace particles
}//namespace graphics