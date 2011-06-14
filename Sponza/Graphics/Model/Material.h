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

	// Binds the textures to the device
	void Bind();
};