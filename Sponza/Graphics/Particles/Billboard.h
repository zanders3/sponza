#pragma once

#include "stdafx.h"

struct BillboardVertex
{
	float x, y;
};

struct BillboardInst
{
	D3DXVECTOR3 mPosition;
	D3DCOLOR	mColor;
	FLOAT		mSize;
};

class Billboard
{
public:
	Billboard(bool useCircle, size_t maxCount);
	~Billboard();

	void Draw();
	void SetBillboards(BillboardInst* pInstances, size_t count);

private:

	void Create(BillboardVertex* vertices, size_t vCount, size_t* indices, size_t iCount);

	ID3D10Buffer*	m_pVertexBuffer;
	ID3D10Buffer*	m_pInstBuffer;
	ID3D10Buffer*	m_pIndexBuffer;

	DWORD			m_instanceCount;
	DWORD			m_indexCount;
	size_t			m_maxCount;

	static D3D10_INPUT_ELEMENT_DESC s_layout[4];
};