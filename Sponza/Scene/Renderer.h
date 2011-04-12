#pragma once
#include "Scene/SceneList.h"
#include "Graphics/Shader.h"
#include "Graphics/RenderTexture.h"
#include "Scene/SceneList.h"
#include <memory>

class Renderer;
typedef std::shared_ptr<Renderer> RendererPtr;

class Renderer
{
public:
	enum Pass
	{
		Normals,
		LightPrePass,
		Color
	};

	Renderer(ID3D10Device* pDevice, Shader* pShader, SceneListPtr sceneList);

	void Draw();

private:
	void DrawPass(Pass pass);

	Pass			m_pass;
	ID3D10Device*	m_pDevice;

	Shader*			m_pShader;

	DepthTexture	m_frameDepth;

	RenderTexture	m_frameBuffer;
	RenderTexture	m_lightBuffer;
	
	SceneListPtr	m_sceneList;
};
