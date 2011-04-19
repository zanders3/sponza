#include "stdafx.h"
#include "Graphics/Shader.h"

using namespace std;

//-------------------------------------------------
// Statics
//-------------------------------------------------

std::vector<Shader*> Shader::s_shaderList;
size_t				 ShaderPass::s_maxID = 0;
ShaderPass*			ShaderPass::s_pCurrent = nullptr;

//-------------------------------------------------
// Shader Class Implementation
//-------------------------------------------------

Shader::Shader() :
	m_pEffect(nullptr),
	m_pWorld(nullptr),
	m_pView(nullptr),
	m_pProjection(nullptr)
{
}

Shader::~Shader()
{
	SAFE_RELEASE(m_pEffect);
}

void Shader::Load(istream& input)
{
	//Clean up the effect if it's being reloaded
	if (m_pEffect != nullptr)
	{
		m_pEffect->Release();
		m_passes.clear();
	}
	else
	{
		s_shaderList.push_back(this);
	}

	//Read file into memory
	char * data;
	size_t length;
	{
		input.seekg(0, ios::end);
		length = static_cast<size_t>(input.tellg());
		input.seekg(0, ios::beg);

		data = new char[length];
		input.read(data, length);
	}

	//Create effect file
	HRESULT hr;
	V(D3D10CreateEffectFromMemory(data, length, 0, m_pDevice, NULL, &m_pEffect));
	delete[] data;

	//Load base effect parameters
	m_pWorld =		m_pEffect->GetVariableByName("World")->AsMatrix();
	m_pView =		m_pEffect->GetVariableByName("View")->AsMatrix();
	m_pProjection = m_pEffect->GetVariableByName("Projection")->AsMatrix();

	//Load effect passes
	{
		ID3D10EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
		D3D10_TECHNIQUE_DESC desc;
		pTechnique->GetDesc(&desc);

		for (size_t i = 0; i<desc.Passes; ++i)
		{
			m_passes.push_back(ShaderPass(pTechnique->GetPassByIndex(i)));
		}
	}
}

void Shader::SetWorld(const D3DXMATRIX& world)
{
	m_pWorld->SetMatrix((float*)&world);
}

void Shader::SetView(const D3DXMATRIX& view)
{
	size_t size = s_shaderList.size();
	for (size_t i = 0; i<size; ++i)
		s_shaderList[i]->m_pView->SetMatrix((float*)&view);
}

void Shader::SetProjection(const D3DXMATRIX& projection)
{
	size_t size = s_shaderList.size();
	for (size_t i = 0; i<size; ++i)
		s_shaderList[i]->m_pProjection->SetMatrix((float*)&projection);
}

//-------------------------------------------------
// Shader Pass Class Implementation
//-------------------------------------------------

ShaderPass::ShaderPass(ID3D10EffectPass* pPass) :
	m_pPass(pPass),
	m_id(s_maxID++)
{
}

ShaderPass::~ShaderPass()
{
}

void ShaderPass::Bind()
{
	s_pCurrent = this;
	m_pPass->Apply(0);
}