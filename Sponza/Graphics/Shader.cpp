// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/Shader.h"
#include "Content/ContentReader.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace graphics
{

//----------------------------------------------------------------------------------------
// Static Data
//----------------------------------------------------------------------------------------
std::map<size_t, EffectPool*> Shader::s_effectPools;
size_t				ShaderPass::s_maxID = 0;
ShaderPass*			ShaderPass::s_pCurrent = nullptr;

//----------------------------------------------------------------------------------------
// Effect Pool Class Implementation
//----------------------------------------------------------------------------------------

EffectPool::EffectPool(
) : m_pool(nullptr)
{
}

//----------------------------------------------------------------------------------------

EffectPool::~EffectPool()
{
	SAFE_RELEASE(m_pool);
}

//----------------------------------------------------------------------------------------

void 
EffectPool::Load(
	content::ContentReader& reader
)
{
	//Read file into memory
	char * data = reader.Read<char>(reader.Size());

	//Create effect pool
	HRESULT hr;
	V(D3D10CreateEffectPoolFromMemory(data, reader.Size(), 0, m_pDevice, &m_pool));
}

//----------------------------------------------------------------------------------------
// Shader Class Implementation
//----------------------------------------------------------------------------------------

Shader::Shader(
) :
	m_pEffect(nullptr),
	m_pWorld(nullptr),
	m_pView(nullptr),
	m_pProjection(nullptr)
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

	//Load the effect pool
	UINT flags = 0;
	ID3D10EffectPool* pPool = NULL;
	//TODO: finish effect pooling stuff. :(
	/*if (headerFileHash != 0)
	{
		flags = D3D10_EFFECT_COMPILE_CHILD_EFFECT;

		auto find = s_effectPools.find(headerFileHash);
		if (find == s_effectPools.end())
		{
			EffectPool* pool = m_pContent->Get<EffectPool>((ContentID::Type)headerFileHash);
			pPool = pool->m_pool;
			s_effectPools.insert(std::make_pair(headerFileHash, pool));
		}
	}*/

	//Read file into memory
	char * data = reader.Read<char>(reader.Size());

	//Create effect file
	HRESULT hr;
	V(D3D10CreateEffectFromMemory(data, reader.Size(), flags, m_pDevice, pPool, &m_pEffect));

	//Load base effect parameters
	m_pWorld =		m_pEffect->GetVariableByName("World")->AsMatrix();
	m_pView =		m_pEffect->GetVariableByName("View")->AsMatrix();
	m_pProjection = m_pEffect->GetVariableByName("Projection")->AsMatrix();

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

void Shader::SetWorld(const D3DXMATRIX& world)
{
	m_pWorld->SetMatrix((float*)&world);
}

//----------------------------------------------------------------------------------------

void Shader::SetView(const D3DXMATRIX& view)
{
	/*size_t size = s_shaderList.size();
	for (size_t i = 0; i<size; ++i)
		s_shaderList[i]->m_pView->SetMatrix((float*)&view);*/
}

//----------------------------------------------------------------------------------------

void Shader::SetProjection(const D3DXMATRIX& projection)
{
	/*size_t size = s_shaderList.size();
	for (size_t i = 0; i<size; ++i)
		s_shaderList[i]->m_pProjection->SetMatrix((float*)&projection);*/
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