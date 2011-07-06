// -----------------------------------------------------------------------------
//	Copyright Alex Parker � 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/DepthTexture.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace graphics
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

DepthTexture::DepthTexture(
)
{
	m_pDevice = GetDevice();

	m_pDepthStencilView = DXUTGetD3D10DepthStencilView();
	m_isDXUTDepthStencil = true;
}

// -----------------------------------------------------------------------------

DepthTexture::~DepthTexture()
{
	if (!m_isDXUTDepthStencil)
	{
		SAFE_RELEASE(m_pDepthStencilView);
	}
}

// -----------------------------------------------------------------------------

void 
DepthTexture::Clear()
{
	m_pDevice->ClearDepthStencilView( m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0 );
}

//----------------------------------------------------------------------------------------

void 
DepthTexture::OnResize(
	int newWidth,
	int newHeight
)
{
	if (m_isDXUTDepthStencil)
	{
		m_pDevice = GetDevice();
		m_pDepthStencilView = DXUTGetD3D10DepthStencilView();
	}
}

//----------------------------------------------------------------------------------------

}//namespace graphics