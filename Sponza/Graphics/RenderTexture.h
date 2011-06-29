// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Model
//		- Loads and handles models and associated materials.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include "Graphics/Texture.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace graphics
{

	class DepthTexture;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class RenderTexture : public Texture
{
public:
	// Creates a RenderTexture from the graphics device
	RenderTexture(DepthTexture* pDepth);
	// Creates a new Render Texture
	RenderTexture(DepthTexture* pDepth, DXGI_FORMAT format, int width, int height);
	~RenderTexture();

	void BindRT();
	void Clear(const D3DXVECTOR4& color);

private:
	bool					m_isBaseRenderView;
	ID3D10RenderTargetView* m_pRenderTargetView;
	DepthTexture*			m_pDepthTexture;
};

// -----------------------------------------------------------------------------

}//namespace graphics