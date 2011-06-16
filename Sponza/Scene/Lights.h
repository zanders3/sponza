// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Light
//		- Represents a point light!
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include "Graphics/Particles/Billboard.h"
#include <memory>
#include <array>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace scene
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class Light
{
public:
	friend class LightList;

	void SetSize(float size);

	void SetPosition(const D3DXVECTOR3& position);

	void SetColor(const D3DXCOLOR& color);

private:
	bool*								m_dirty;
	graphics::particles::BillboardInst*	m_vertex;
};

// -----------------------------------------------------------------------------

class LightList
{
public:
	LightList();
	~LightList();

	static const int MaxLights = 64;

	Light* CreateLight();
	void DrawLights();

private:
	bool m_dirty;

	int									m_numLights;
	Light								m_lights[MaxLights];
	graphics::particles::BillboardInst	m_vertices[MaxLights];
	
	graphics::particles::Billboard		m_billboards;
};

// -----------------------------------------------------------------------------

}//namespace scene