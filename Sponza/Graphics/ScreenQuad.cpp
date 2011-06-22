// -----------------------------------------------------------------------------
//	Copyright Alex Parker � 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics\ScreenQuad.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace graphics
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

ScreenQuad::ScreenQuad()
{
	const D3DXVECTOR3 zero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	model::Vertex vertices[] =
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

	Create((model::Vertex*)&vertices, 4, (size_t*)&indices, 6, GetDevice());
}

// -----------------------------------------------------------------------------

void ScreenQuad::Draw()
{
	static ScreenQuad quad;

	GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//quad.Mesh::Draw();
}

//----------------------------------------------------------------------------------------

}//namespace graphics