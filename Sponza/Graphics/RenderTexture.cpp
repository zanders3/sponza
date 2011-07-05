// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/RenderTexture.h"
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

RenderTexture::RenderTexture(
	DepthTexture* pDepth
)
{
	m_pDepthTexture = pDepth;

	m_pRenderTargetView = DXUTGetD3D10RenderTargetView();
	m_pDevice = GetDevice();
	m_isBaseRenderView = true;
}

//----------------------------------------------------------------------------------------

RenderTexture::RenderTexture(DepthTexture* pDepth, DXGI_FORMAT format, int width, int height)
{
	ID3D10Device* pDevice = GetDevice();
	m_pDepthTexture = pDepth;
	m_pDevice = GetDevice();
	m_isBaseRenderView = false;

	// Create RT Texture
	HRESULT hr;
	D3D10_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;

	V(pDevice->CreateTexture2D(&desc, NULL, &m_pTexture));

	// Create RT View
	D3D10_RENDER_TARGET_VIEW_DESC rtDesc;
	rtDesc.Format = desc.Format;
	rtDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
	rtDesc.Texture2D.MipSlice = 0;

	V(pDevice->CreateRenderTargetView( m_pTexture, &rtDesc, &m_pRenderTargetView ));

	// Create the shader-resource view
	D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;

	V(pDevice->CreateShaderResourceView( m_pTexture, &srDesc, &m_pTextureView ));
}

//----------------------------------------------------------------------------------------

RenderTexture::~RenderTexture()
{
	if (!m_isBaseRenderView)
	{
		SAFE_RELEASE(m_pRenderTargetView);
	}
}

//----------------------------------------------------------------------------------------

void RenderTexture::BindRT()
{
	m_pDevice->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthTexture->m_pDepthStencilView );
}

//----------------------------------------------------------------------------------------

void RenderTexture::Clear(
	const D3DXVECTOR4& color
)
{
	m_pDevice->ClearRenderTargetView( m_pRenderTargetView, (float*)&color );
	m_pDepthTexture->Clear();
}

//----------------------------------------------------------------------------------------

}//namespace graphics