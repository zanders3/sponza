#pragma once
#include "stdafx.h"

#include "Content/ContentItem.h"
#include <fstream>

class Texture : public ContentItem
{
public:
	enum BindType
	{
		Diffuse,
		Normal
	};

	Texture();
	~Texture();

	void Load(std::istream& input);

	void Bind(const BindType& type);

private:
	ID3D10ShaderResourceView* m_pTexture;
};