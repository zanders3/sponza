#pragma once
#include "stdafx.h"
#include "Content/ContentItem.h"
#include <fstream>

class Vertex
{
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mNormal;
	D3DXVECTOR3 mTangent;
	D3DXVECTOR2 mTexCoord;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Create(Vertex* pVertex, size_t numVertices, size_t* pIndices, size_t numIndices, ID3D10Device* pDevice);
	void Load(std::istream& input, ID3D10Device* pDevice);

	void Draw(ID3D10Device* pDevice);
private:
	ID3D10Buffer* mVertexBuffer;
	ID3D10Buffer* mIndexBuffer;
	size_t		  mNumIndices;
};

class Model : public ContentItem
{
public:
	Model();
	~Model();

public:
	void Load(std::ifstream& input);
	void Draw();

private:
	unsigned int mNumMeshes;
	Mesh * mMeshes;
};