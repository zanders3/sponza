#pragma once
#include "stdafx.h"
#include "Content/ContentItem.h"
#include <fstream>

#include "Graphics/Texture.h"

class Material
{
public:
	void Load(std::istream& input, ContentLoader* pLoader);

	Texture* mDiffuse;
	Texture* mNormal;

	void Bind();

	Texture* Material::GetDiffuseDefault(ID3D10Device* pDevice);
	Texture* Material::GetNormalDefault(ID3D10Device* pDevice);
};