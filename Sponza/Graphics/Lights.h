#pragma once

#include "stdafx.h"
#include "Graphics/Particles/Billboard.h"
#include <memory>
#include <array>

class LightList;

class Light
{
public:
	friend LightList;

	void SetSize(float size)
	{
		*m_dirty = true;
		m_vertex->mSize = size;
	}

	void SetPosition(const D3DXVECTOR3& position)
	{
		*m_dirty = true;
		m_vertex->mPosition = position;
	}

	void SetColor(const D3DXCOLOR& color)
	{
		*m_dirty = true;
		m_vertex->mColor = color;
	}

private:
	bool*			m_dirty;
	BillboardInst*	m_vertex;
};

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

	int				m_numLights;
	Light			m_lights[MaxLights];
	BillboardInst	m_vertices[MaxLights];
	
	Billboard		m_billboards;
};