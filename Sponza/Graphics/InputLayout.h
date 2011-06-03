#pragma once
#include "stdafx.h"
#include <map>

class InputLayout
{
public:
	InputLayout(D3D10_INPUT_ELEMENT_DESC* pDesc, size_t count);
	~InputLayout();

	void Bind();

private:
	D3D10_INPUT_ELEMENT_DESC*				m_pDesc;
	size_t									m_descCount;

	std::map<size_t, ID3D10InputLayout*>	m_layouts;
};