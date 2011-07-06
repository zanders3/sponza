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

namespace
{
	game::Notifier<graphics::ResizeArgs>	s_notifier;
}

namespace graphics
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

RenderTexture::RenderTexture(
	DepthTexture* pDepth
)
{
	m_resizeHandler.reset(new game::Observer<ResizeArgs>([=](const ResizeArgs& args)
	{
		this->m_pRenderTargetView = DXUTGetD3D10RenderTargetView();
		this->m_pDevice = GetDevice();
		this->m_pDepthTexture->OnResize(args.newWidth, args.newHeight);
	}));
	s_notifier.Register(m_resizeHandler.get());

	m_pDepthTexture = pDepth;

	m_pRenderTargetView = DXUTGetD3D10RenderTargetView();
	m_pDevice = GetDevice();
	m_isBaseRenderView = true;
}

//----------------------------------------------------------------------------------------

RenderTexture::RenderTexture(DepthTexture* pDepth, DXGI_FORMAT format, int width, int height, bool resizeWithScreen)
{
	m_resizeHandler.reset(new game::Observer<ResizeArgs>([=](const ResizeArgs& args)
	{
		SAFE_RELEASE(this->m_pRenderTargetView);

		this->m_pDevice = GetDevice();
		this->Setup(format, resizeWithScreen ? args.newWidth : width, resizeWithScreen ? args.newHeight : height);
		this->m_pDepthTexture->OnResize(args.newWidth, args.newHeight);
	}));

	m_pDepthTexture = pDepth;
	m_pDevice = GetDevice();
	m_isBaseRenderView = false;

	Setup(format, width, height);
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

void 
RenderTexture::BindRT()
{
	m_pDevice->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDepthTexture->m_pDepthStencilView );
}

//----------------------------------------------------------------------------------------

void 
RenderTexture::Clear(
	const D3DXVECTOR4& color
)
{
	m_pDevice->ClearRenderTargetView( m_pRenderTargetView, (float*)&color );
	m_pDepthTexture->Clear();
}

//----------------------------------------------------------------------------------------

void
RenderTexture::OnResize(
	int newWidth, 
	int newHeight
)
{
	ResizeArgs args =
	{
		newWidth,
		newHeight
	};
	s_notifier.Notify(args);
}

//----------------------------------------------------------------------------------------

void
RenderTexture::Setup(DXGI_FORMAT format, int width, int height)
{
	ID3D10Device* pDevice = GetDevice();

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

}//namespace graphics