#include "stdafx.h"
#include "Graphics/DepthTexture.h"

DepthTexture::DepthTexture(ID3D10Device* pDevice)
{
	m_pDevice = pDevice;

	m_pDepthStencilView = DXUTGetD3D10DepthStencilView();
	m_isDXUTDepthStencil = true;
}

DepthTexture::~DepthTexture()
{
	if (!m_isDXUTDepthStencil)
	{
		SAFE_RELEASE(m_pDepthStencilView);
	}
}

void DepthTexture::Clear()
{
	m_pDevice->ClearDepthStencilView( m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0 );
}