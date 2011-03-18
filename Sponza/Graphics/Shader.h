#pragma once
#include "stdafx.h"
#include "Content/ContentItem.h"
#include <fstream>
#include <vector>

class ShaderPass
{
public:
	ShaderPass(ID3D10EffectPass* pPass, ID3D10InputLayout* pLayout);
	~ShaderPass();

	void Bind();

private:
	ID3D10EffectPass*	m_pPass;
	ID3D10InputLayout*	m_pLayout;
};

class Shader
{
public:
	Shader();
	~Shader();

	void Load(std::ifstream& input);

	inline void Bind(int pass) { m_passes[pass].Bind();  }
	inline int  NumPasses()	   { return m_passes.size(); }

	inline void SetWorld(const D3DXMATRIX& world);

	static void SetView(const D3DXMATRIX& view);
	static void SetProjection(const D3DXMATRIX& projection);

private:
	ID3D10Effect*			m_pEffect;
	std::vector<ShaderPass> m_passes;

	ID3D10EffectVariable *m_pWorld, *m_pView, *m_pProjection;

	static std::vector<Shader*> s_shaderList;
};