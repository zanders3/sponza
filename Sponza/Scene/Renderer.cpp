#include "stdafx.h"
#include "Scene/Renderer.h"
#include "Graphics/ScreenQuad.h"

Renderer::Renderer(ID3D10Device* pDevice, Shader* pShader, SceneListPtr sceneList) :
	m_pDevice(pDevice),
	m_pShader(pShader),
	m_frameDepth(pDevice),
	m_frameBuffer(pDevice, &m_frameDepth),
	m_normalBuffer(pDevice, &m_frameDepth, DXGI_FORMAT_B8G8R8A8_UNORM, DXUTGetWindowWidth(), DXUTGetWindowHeight()),
	m_lightBuffer(pDevice, &m_frameDepth, DXGI_FORMAT_B8G8R8A8_UNORM, DXUTGetWindowWidth(), DXUTGetWindowHeight()),
	m_sceneList(sceneList)
{
}

void Renderer::Draw()
{
	static const D3DXVECTOR4 clearColor = D3DXVECTOR4( 0.0f, 0.3f, 1.0f, 1.0f );

	{
		PIXEvent(L"Normal Pass");
		m_lightBuffer.BindRT();
		m_lightBuffer.Clear(clearColor);

		m_pShader->Bind(Pass::Normals);
		m_sceneList->Draw(m_pShader);
	}

	/*{
		PIXEvent(L"Lights Pass");
		m_frameBuffer.BindRT();
		m_frameBuffer.Clear(clearColor);

		m_pShader->Bind(Pass::Lights);
		m_lightBuffer.Bind(Texture::Diffuse);
		m_sceneList->DrawLights();
	}*/

	/*{
		PIXEvent(L"Fullscreen");
		m_frameBuffer.BindRT();

		m_pShader->Bind(Pass::Color);
		m_lightBuffer.Bind(Texture::Diffuse);
		ScreenQuad::Draw(m_pDevice);

		Texture::GetDiffuseDefault(m_pDevice)->Bind(Texture::Diffuse);
	}*/
}

void Renderer::DrawPass(Pass pass)
{
	
}