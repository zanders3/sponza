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
	Texture(ID3D10Device* pDevice, D3DCOLOR pixel);
	virtual ~Texture();

	void Load(std::istream& input);
	void Bind(const BindType& type);

	static Texture* GetDiffuseDefault(ID3D10Device* pDevice);
	static Texture* GetNormalDefault(ID3D10Device* pDevice);

protected:
	void CreatePixelTexture(D3DCOLOR color);

	ID3D10ShaderResourceView* m_pTextureView;
	ID3D10Texture2D*		  m_pTexture;
};