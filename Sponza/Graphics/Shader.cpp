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

namespace
{
	u32 shaderID = 0;
}

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
) : m_id(shaderID++),
	m_drawOrder(0),
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
	V(D3D10CreateEffectFromMemory(data, size, 0, m_pDevice, nullptr, &m_pEffect));

	//Reset all shader parameters
	for (auto paramsIter = m_params.begin(); paramsIter != m_params.end(); ++paramsIter)
	{
		auto& paramMap = paramsIter->m_paramMap;
		for (auto param = paramMap.begin(); param != paramMap.end(); ++param)
		{
			param->second.SetVariable(m_pEffect->GetVariableByName(param->first.c_str()));
		}
	}

	//Register with the global shader params
	GlobalShaderParams::RegisterShader(this);

	m_pWorld = m_pEffect->GetVariableByName("World")->AsMatrix();

	//Load effect passes
	{
		ID3D10EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
		D3D10_TECHNIQUE_DESC desc;
		pTechnique->GetDesc(&desc);

		ID3D10EffectVariable* drawOrder = pTechnique->GetAnnotationByName("DrawOrder");
		if (drawOrder)
		{
			drawOrder->AsScalar()->GetInt((int*)&m_drawOrder);
		}
		
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

ShaderParams*
Shader::CreateShaderParams()
{
	m_params.push_back(ShaderParams(*this));
	return &m_params.back();
}

//----------------------------------------------------------------------------------------

bool
Shader::operator <(
	const Shader& other
) const
{
	return m_drawOrder < other.m_drawOrder && m_id < other.m_id;
}

//----------------------------------------------------------------------------------------

bool
Shader::operator ==(
	const Shader& other
) const
{
	return m_id == other.m_id;
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