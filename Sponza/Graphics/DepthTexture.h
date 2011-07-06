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

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class DepthTexture : public Texture
{
public:
	friend class RenderTexture;//to permit access to m_pDepthStencilView

	DepthTexture();
	//DepthTexture(ID3D10Device* pDevice, int width, int height);
	~DepthTexture();

	void 
	BindDepth();
	
	void 
	Clear();

	void 
	OnResize(
		int newWidth,
		int newHeight
	);
private:
	bool					m_isDXUTDepthStencil;
	ID3D10DepthStencilView* m_pDepthStencilView;
};

// -----------------------------------------------------------------------------

}//namespace graphics