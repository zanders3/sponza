#include "stdafx.h"
#include "Model.h"

using namespace std;

Model::Model() :
	mMeshes(nullptr)
{
}

Model::~Model()
{
	if (mMeshes) delete[] mMeshes;
}

void Model::Load(std::istream& input)
{
	if (mMeshes) delete[] mMeshes;

	input.read((char*)&mNumMeshes, sizeof(size_t));

	mMeshes = new Mesh[mNumMeshes];
	for (size_t i = 0; i<mNumMeshes; ++i)
	{
		mMeshes[i].Load(input, m_pDevice);
	}
}

void Model::Draw()
{
	m_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (size_t i = 0; i<mNumMeshes; ++i)
		mMeshes[i].Draw(m_pDevice);
}

//----------------------------------------------------------------------------------------

Mesh::Mesh() :
	mVertexBuffer(nullptr),
	mIndexBuffer(nullptr)
{
}

Mesh::~Mesh()
{
	SAFE_RELEASE(mVertexBuffer);
	SAFE_RELEASE(mIndexBuffer);
}

void Mesh::Load(std::istream& input, ID3D10Device* pDevice)
{
	size_t	 mNumVertices;
	Vertex * mVertices;
	size_t	 mNumIndices;
	size_t * mIndices;

	input.read((char*)&mNumVertices, sizeof(size_t));

	mVertices = new Vertex[mNumVertices];
	input.read((char*)mVertices, sizeof(Vertex)*mNumVertices);

	input.read((char*)&mNumIndices, sizeof(size_t));

	mIndices = new size_t[mNumIndices];
	input.read((char*)mIndices, sizeof(size_t)*mNumIndices);

	Create(mVertices, mNumVertices, mIndices, mNumIndices, pDevice);

	delete[] mIndices;
	delete[] mVertices;
}

void Mesh::Create(Vertex* pVertex, size_t numVertices, size_t* pIndices, size_t numIndices, ID3D10Device* pDevice)
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

void Mesh::Draw(ID3D10Device* pDevice)
{
	pDevice->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	pDevice->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

	pDevice->DrawIndexed(mNumIndices, 0, 0);
}