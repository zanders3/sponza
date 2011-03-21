#include "stdafx.h"
#include "Graphics/Shader.h"

using namespace std;

std::vector<Shader*> Shader::s_shaderList;
D3D10_INPUT_ELEMENT_DESC Shader::s_layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, 36, D3D10_INPUT_PER_VERTEX_DATA, 0 }
};

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

void Shader::Load(ifstream& input)
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
		ID3D10InputLayout* pLayout;
		pTechnique->GetDesc(&desc);

		for (size_t i = 0; i<desc.Passes; ++i)
		{
			ID3D10EffectPass* pPass = pTechnique->GetPassByIndex(i);
			D3D10_PASS_DESC passDesc;
			pPass->GetDesc(&passDesc);

			V(m_pDevice->CreateInputLayout(s_layout, 4, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pLayout));

			m_passes.push_back(std::unique_ptr<ShaderPass>(new ShaderPass(m_pDevice, pPass, pLayout)));
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

//----------------------------------------------------------------------------

ShaderPass::ShaderPass(ID3D10Device* pDevice, ID3D10EffectPass* pPass, ID3D10InputLayout* pLayout) :
	m_pDevice(pDevice),
	m_pPass(pPass),
	m_pLayout(pLayout)
{
}

ShaderPass::~ShaderPass()
{
	SAFE_RELEASE(m_pLayout);
}

void ShaderPass::Bind()
{
	m_pDevice->IASetInputLayout(m_pLayout);
	m_pPass->Apply(0);
}