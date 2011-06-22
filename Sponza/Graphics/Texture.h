// -----------------------------------------------------------------------------
//	Copyright Alex Parker � 2011
//	
//	Texture
//		- Base class for all textures!
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include "Content/ContentItem.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace graphics
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class Texture : public content::ContentItem
{
public:
	friend struct ShaderParam;

	enum BindType
	{
		Diffuse,
		Normal
	};

	Texture();
	Texture(
		D3DCOLOR pixel
	);

	virtual 
	~Texture();

	virtual void Load(
		content::ContentReader& reader
	);

	void Bind(const BindType& type);

	static Texture* 
	GetDiffuseDefault();

	static Texture* 
	GetNormalDefault();

protected:
	void CreatePixelTexture(D3DCOLOR color);

	ID3D10ShaderResourceView* m_pTextureView;
	ID3D10Texture2D*		  m_pTexture;
};

// -----------------------------------------------------------------------------

}//namespace graphics