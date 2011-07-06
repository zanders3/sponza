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
#include "Observer.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace graphics
{

	class DepthTexture;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

struct ResizeArgs
{
	int newWidth;
	int newHeight;
};

class RenderTexture : public Texture
{
public:
	// Creates a RenderTexture from the graphics device
	RenderTexture(DepthTexture* pDepth);
	// Creates a new Render Texture
	RenderTexture(DepthTexture* pDepth, DXGI_FORMAT format, int width, int height, bool resizeWithScreen = true);
	~RenderTexture();

	void BindRT();
	void Clear(const D3DXVECTOR4& color);

	static void
	OnResize(int newWidth, int newHeight);

private:
	void
	Setup(DXGI_FORMAT format, int width, int height);

	std::unique_ptr<game::Observer<ResizeArgs>>	m_resizeHandler;
	bool										m_isBaseRenderView;
	ID3D10RenderTargetView*						m_pRenderTargetView;
	DepthTexture*								m_pDepthTexture;
};

// -----------------------------------------------------------------------------

}//namespace graphics