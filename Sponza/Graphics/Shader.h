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
#include <list>

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
	u32					m_id;

	static u32			s_maxID;
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

	bool 
	Bind(u32 passHash);

	int 
	NumPasses()	   
	{ 
		return m_passes.size(); 
	}

	void
	SetWorld(
		D3DXMATRIX& world
	);

	ShaderParams*
	CreateShaderParams();

	bool
	operator <(
		const Shader& other
	) const;

	bool
	operator ==(
		const Shader& other
	) const;

private:
	u32												m_id;
	u32												m_drawOrder;
	ID3D10EffectMatrixVariable*						m_pWorld;
	ID3D10Effect*									m_pEffect;
	std::map<u32, ShaderPass>						m_passes;
	std::list<ShaderParams>							m_params;
};

// -----------------------------------------------------------------------------

}//namespace graphics