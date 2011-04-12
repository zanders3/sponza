#pragma once

#include "stdafx.h"
#include "Graphics\Model.h"

class ScreenQuad : Mesh
{
public:
	static void Draw(ID3D10Device* pDevice);

private:
	ScreenQuad(ID3D10Device* pDevice);

	ID3D10Buffer*		m_pQuad;
};