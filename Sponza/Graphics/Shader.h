#pragma once
#include "stdafx.h"

#include "Content/ContentItem.h"
#include "Graphics/InputLayout.h"

#include <fstream>
#include <vector>
#include <array>
#include <memory>

class ShaderPass
{
public:
	friend InputLayout;

	ShaderPass(ID3D10EffectPass* pPass);
	~ShaderPass();

	void Bind();

private:
	ID3D10EffectPass*	m_pPass;
	size_t				m_id;

	static size_t		s_maxID;
	static ShaderPass*	s_pCurrent;
};

class EffectPool : public ContentItem
{
public:
	EffectPool();
	~EffectPool();

	void Load(std::istream& input);

	ID3D10EffectPool*	m_pool;
};

class Shader : public ContentItem
{
public:
	Shader();
	~Shader();

	void Load(std::istream& input);

	inline void Bind(int pass) { m_passes[pass].Bind(); }
	inline int  NumPasses()	   { return m_passes.size(); }

	void SetWorld(const D3DXMATRIX& world);
	static void SetView(const D3DXMATRIX& view);
	static void SetProjection(const D3DXMATRIX& projection);

private:
	ID3D10Effect*									m_pEffect;
	std::vector<ShaderPass>							m_passes;

	ID3D10EffectMatrixVariable *m_pWorld, *m_pView, *m_pProjection;
	static std::map<size_t, EffectPool*> s_effectPools;
};