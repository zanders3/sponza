#include "stdafx.h"
#include "Graphics\ScreenQuad.h"

ScreenQuad::ScreenQuad(ID3D10Device* pDevice)
{
	const D3DXVECTOR3 zero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Vertex vertices[] =
	{
		{
			D3DXVECTOR3(-1.0f, -1.0f, 0.0f),
			zero,
			zero,
			zero,
			D3DXVECTOR2(0.0f, 1.0f)
		},
		{
			D3DXVECTOR3(-1.0f, 1.0f, 0.0f),
			zero,
			zero,
			zero,
			D3DXVECTOR2(0.0f, 0.0f)
		},
		{
			D3DXVECTOR3(1.0f, 1.0f, 0.0f),
			zero,
			zero,
			zero,
			D3DXVECTOR2(1.0f, 0.0f)
		},
		{
			D3DXVECTOR3(1.0f, -1.0f, 0.0f),
			zero,
			zero,
			zero,
			D3DXVECTOR2(1.0f, 1.0f)
		}
	};

	size_t indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	Create((Vertex*)&vertices, 4, (size_t*)&indices, 6, pDevice);
}

void ScreenQuad::Draw(ID3D10Device* pDevice)
{
	static ScreenQuad quad(pDevice);

	quad.Mesh::Draw(pDevice);
}