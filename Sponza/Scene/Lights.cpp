// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Scene/Lights.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;
using namespace graphics::particles;

namespace scene
{

// -----------------------------------------------------------------------------
// Light Class Implementation
// -----------------------------------------------------------------------------

void 
Light::SetSize(
	float size
)
{
	*m_dirty = true;
	m_vertex->mSize = size;
}

//---------------------------------------------------------------------------------------

void 
Light::SetPosition(
	const D3DXVECTOR3& position
)
{
	*m_dirty = true;
	m_vertex->mPosition = position;
}

//---------------------------------------------------------------------------------------

void 
Light::SetColor(
	const D3DXCOLOR& color
)
{
	*m_dirty = true;
	m_vertex->mColor = color;
}

// -----------------------------------------------------------------------------
// Light List Class Implementation
// -----------------------------------------------------------------------------

LightList::LightList(
) : m_dirty(true),
	m_numLights(0),
	m_billboards(true, MaxLights)
{
}

//---------------------------------------------------------------------------------------

LightList::~LightList()
{
}

//---------------------------------------------------------------------------------------

Light* 
LightList::CreateLight()
{
	if (m_numLights >= MaxLights)
		return nullptr;
	else
	{
		m_lights[m_numLights].m_dirty = &m_dirty;
		m_lights[m_numLights].m_vertex = &m_vertices[m_numLights];
		m_lights[m_numLights].m_vertex->mSize = 100.0f;

		m_dirty = true;

		return &m_lights[m_numLights++];
	}
}

//---------------------------------------------------------------------------------------

void 
LightList::DrawLights()
{
	if (m_numLights == 0) return;

	//Update the light list
	if (m_dirty)
	{
		m_billboards.SetBillboards((BillboardInst*)&m_vertices, m_numLights);
		m_dirty = false;
	}

	//Draw the light list
	m_billboards.Draw();
}

//---------------------------------------------------------------------------------------

}//namespace model