#include "stdafx.h"
#include "Graphics/Shader.h"

//-------------------------------------------------
// Input Layout Implementation
//-------------------------------------------------

InputLayout::InputLayout(D3D10_INPUT_ELEMENT_DESC* pDesc, size_t count) :
	m_pDesc(pDesc),
	m_descCount(count)
{
}

InputLayout::~InputLayout()
{
	auto end = m_layouts.end();
	for (auto iter = m_layouts.begin(); iter != end; ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
}

void InputLayout::Bind()
{
	auto iter = m_layouts.find(ShaderPass::s_pCurrent->m_id);
	if (iter != m_layouts.end())
	{
		GetDevice()->IASetInputLayout(iter->second);
	}
	else
	{
		D3D10_PASS_DESC passDesc;
		ShaderPass::s_pCurrent->m_pPass->GetDesc(&passDesc);

		ID3D10InputLayout* pLayout = nullptr;
		if (passDesc.pIAInputSignature != nullptr)
		{
			HRESULT hr;
			V(GetDevice()->CreateInputLayout(m_pDesc, m_descCount, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pLayout));
		}

		GetDevice()->IASetInputLayout(pLayout);
		m_layouts.insert(std::pair<size_t, ID3D10InputLayout*>(ShaderPass::s_pCurrent->m_id, pLayout));
	}
}