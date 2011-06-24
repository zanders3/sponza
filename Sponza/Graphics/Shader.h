// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Shader
//		- Handles shader effects
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"
#include "Content/ContentItem.h"
#include "Graphics/InputLayout.h"
#include <vector>
#include <array>
#include <memory>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{
	class ContentReader;
}

namespace graphics
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------

class Shader : public content::ContentItem
{
public:
	friend class ShaderParams;

	Shader();
	~Shader();

	virtual void 
	Load(
		content::ContentReader& reader
	);

	void 
	Bind(int pass) 
	{ 
		m_passes[pass].Bind(); 
	}

	int 
	NumPasses()	   
	{ 
		return m_passes.size(); 
	}

	void
	SetWorld(
		D3DXMATRIX& world
	);

private:
	ID3D10EffectMatrixVariable*						m_pWorld;
	ID3D10Effect*									m_pEffect;
	std::vector<ShaderPass>							m_passes;
};

// -----------------------------------------------------------------------------

}//namespace graphics