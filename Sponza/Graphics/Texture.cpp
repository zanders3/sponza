// -----------------------------------------------------------------------------
//	Copyright Alex Parker � 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/Texture.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace graphics
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

Texture::Texture(
) : m_pTextureView(nullptr),
	m_pTexture(nullptr)
{
}

//----------------------------------------------------------------------------------------

Texture::Texture(
	D3DCOLOR pixel
) : m_pTextureView(nullptr),
	m_pTexture(nullptr)
{
	CreatePixelTexture(pixel);
}

//----------------------------------------------------------------------------------------

Texture::~Texture()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTextureView);
}

//----------------------------------------------------------------------------------------

void 
Texture::Load(
	content::ContentReader& reader
)
{
	//Read data
	u32 length = reader.Size();
	char * data = reader.Read<char>(length);

	//Create texture
	HRESULT hr;
	V(D3DX10CreateShaderResourceViewFromMemory(
		m_pDevice,
		data,
		length,
		NULL,
		NULL,
		&m_pTextureView,
		NULL));
}

//----------------------------------------------------------------------------------------

void
Texture::CreatePixelTexture(
	D3DCOLOR color
)
{
	HRESULT hr;
	//Create the texture
	D3D10_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = 1;
	desc.Height = 1;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D10_USAGE_DYNAMIC;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	ID3D10Texture2D *pTexture = NULL;

	V(m_pDevice->CreateTexture2D( &desc, NULL, &pTexture ));

	// Set the color
	D3D10_MAPPED_TEXTURE2D mappedTex;
	V(pTexture->Map( D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex ));

	D3DCOLOR* pTexels = (D3DCOLOR*)mappedTex.pData;
	*pTexels = color;

	pTexture->Unmap( D3D10CalcSubresource(0, 0, 1) );

	m_pTexture = pTexture;

	// Create the shader-resource view
	D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels = 1;

	ID3D10ShaderResourceView *pShaderResView = NULL;
	V(m_pDevice->CreateShaderResourceView( pTexture, &srDesc, &m_pTextureView ));
}

//----------------------------------------------------------------------------------------

Texture* 
Texture::GetDiffuseDefault()
{
	static Texture texture(D3DCOLOR_ARGB(255, 255, 0, 255));

	return &texture;
}

//----------------------------------------------------------------------------------------

Texture* 
Texture::GetNormalDefault()
{
	static Texture texture(D3DCOLOR_RGBA(255, 128, 128, 255));

	return &texture;
}

//----------------------------------------------------------------------------------------

Texture*
Texture::GetOpacityDefault()
{
	static Texture texture(D3DCOLOR_RGBA(255, 255, 255, 255));

	return &texture;
}

//----------------------------------------------------------------------------------------

}//namespace graphics
