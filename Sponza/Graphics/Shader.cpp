// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/Shader.h"
#include "Graphics/Shader/ShaderParams.h"
#include "Content/ContentReader.h"
#include "Content/ContentManager.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace graphics
{

//----------------------------------------------------------------------------------------
// Static Data
//----------------------------------------------------------------------------------------
size_t				ShaderPass::s_maxID = 0;
ShaderPass*			ShaderPass::s_pCurrent = nullptr;

//----------------------------------------------------------------------------------------
// Shader Class Implementation
//----------------------------------------------------------------------------------------

Shader::Shader(
) :
	m_pEffect(nullptr)
{
}

//----------------------------------------------------------------------------------------

Shader::~Shader()
{
	SAFE_RELEASE(m_pEffect);
}

//----------------------------------------------------------------------------------------

void 
Shader::Load(
	content::ContentReader& reader
)
{
	//Clean up the effect if it's being reloaded
	if (m_pEffect != nullptr)
	{
		m_pEffect->Release();
		m_passes.clear();
	}

	//Read file into memory
	u32 size = reader.Size();
	char * data = reader.Read<char>(size);

	//Create effect file
	HRESULT hr;
	V(D3D10CreateEffectFromMemory(data, size, 0, m_pDevice,/* pool->m_pool*/nullptr, &m_pEffect));

	//Register with the global shader params
	GlobalShaderParams::RegisterShader(this);

	m_pWorld = m_pEffect->GetVariableByName("Position")->AsMatrix();

	//Load effect passes
	{
		ID3D10EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
		D3D10_TECHNIQUE_DESC desc;
		pTechnique->GetDesc(&desc);
		
		m_passes.reserve(desc.Passes);
		for (size_t i = 0; i<desc.Passes; ++i)
		{
			m_passes.push_back(ShaderPass(pTechnique->GetPassByIndex(i)));
		}
	}
}

//----------------------------------------------------------------------------------------

void
Shader::SetWorld(
	D3DXMATRIX& world
)
{
	m_pWorld->SetMatrix((float*)world);
}

//----------------------------------------------------------------------------------------
// Shader Pass Class Implementation
//----------------------------------------------------------------------------------------

ShaderPass::ShaderPass(ID3D10EffectPass* pPass) :
	m_pPass(pPass),
	m_id(s_maxID++)
{
}

//----------------------------------------------------------------------------------------

ShaderPass::~ShaderPass()
{
}

//----------------------------------------------------------------------------------------

void ShaderPass::Bind()
{
	s_pCurrent = this;
	m_pPass->Apply(0);
}

//----------------------------------------------------------------------------------------

}//namespace graphics