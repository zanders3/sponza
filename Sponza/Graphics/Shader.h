#pragma once
#include "stdafx.h"
#include "Content/ContentItem.h"
#include <fstream>
#include <vector>

class ShaderPass
{
public:
	ShaderPass(ID3D10Device* pDevice, ID3D10EffectPass* pPass, ID3D10InputLayout* pLayout);
	~ShaderPass();

	void Bind();

private:
	ID3D10Device*		m_pDevice;
	ID3D10EffectPass*	m_pPass;
	ID3D10InputLayout*	m_pLayout;
};

class Shader : public ContentItem
{
public:
	Shader();
	~Shader();

	void Load(std::istream& input);

	inline void Bind(int pass) { m_passes[pass]->Bind();  }
	inline int  NumPasses()	   { return m_passes.size(); }

	void SetWorld(const D3DXMATRIX& world);
	static void SetView(const D3DXMATRIX& view);
	static void SetProjection(const D3DXMATRIX& projection);

private:
	ID3D10Effect*			m_pEffect;
	std::vector<std::unique_ptr<ShaderPass>> m_passes;

	ID3D10EffectMatrixVariable *m_pWorld, *m_pView, *m_pProjection;

	static std::vector<Shader*> s_shaderList;
	static D3D10_INPUT_ELEMENT_DESC s_layout[5];
};