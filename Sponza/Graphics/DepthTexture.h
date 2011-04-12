#pragma once

#include "Graphics/Texture.h"

class RenderTexture;

class DepthTexture : public Texture
{
public:
	friend RenderTexture;//to permit access to m_pDepthStencilView

	DepthTexture(ID3D10Device* pDevice);
	//DepthTexture(ID3D10Device* pDevice, int width, int height);
	~DepthTexture();

	void BindDepth();
	void Clear();
private:
	bool					m_isDXUTDepthStencil;
	ID3D10DepthStencilView* m_pDepthStencilView;
};