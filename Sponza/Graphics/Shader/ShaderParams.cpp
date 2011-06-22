// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/Shader/ShaderParams.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace graphics
{

// -----------------------------------------------------------------------------
// Static Data
// -----------------------------------------------------------------------------

std::vector<std::pair<Shader*, std::unique_ptr<ShaderParams>>>	GlobalShaderParams::m_shaderList;

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

ShaderParam::ShaderParam(
	float value
) : type(Type::Float)
{
	fValue[0] = value;
}

// -----------------------------------------------------------------------------

ShaderParam::ShaderParam(
	D3DXVECTOR2& value
) : type(Type::Vector)
{
	fValue[0] = value.x;
	fValue[1] = value.y;
}

// -----------------------------------------------------------------------------

ShaderParam::ShaderParam(
	D3DXVECTOR3& value
) : type(Type::Vector)
{
	fValue[0] = value.x;
	fValue[1] = value.y;
	fValue[2] = value.z;
}

// -----------------------------------------------------------------------------

ShaderParam::ShaderParam(
	D3DXVECTOR4& value
) : type(Type::Vector)
{
	fValue[0] = value.x;
	fValue[1] = value.y;
	fValue[2] = value.z;
	fValue[3] = value.w;
}

// -----------------------------------------------------------------------------

ShaderParam::ShaderParam(
	int value
) : type(Type::Int)
{
	iValue = value;
}

// -----------------------------------------------------------------------------

ShaderParam::ShaderParam(
	Texture* value
) : type(Type::Texture)
{
	tValue = value;
}

// -----------------------------------------------------------------------------

ShaderParam::ShaderParam(
	D3DMATRIX* value
) : type(Type::Matrix)
{
	mValue = value;
}

// -----------------------------------------------------------------------------

void 
ShaderParam::SetVariable(
	ID3D10EffectVariable* var
)
{
	variable = var;

	switch (type)
	{
	case Type::Float:
	case Type::Int:
		sVar = var->AsScalar();
		break;

	case Type::Vector:
		vVar = var->AsVector();
		break;

	case Type::Texture:
		tVar = var->AsShaderResource();
		break;

	case Type::Matrix:
		mVar = var->AsMatrix();
		break;

	default:
		_assert(false);//unhandled case!
		break;
	}
}

// -----------------------------------------------------------------------------

void
ShaderParam::Apply()
{
	switch (type)
	{
	case ShaderParam::Type::Float:
		sVar->SetFloat(fValue[0]);
		break;

	case ShaderParam::Type::Vector:
		vVar->SetFloatVector(fValue);
		break;

	case ShaderParam::Type::Int:
		sVar->SetInt(iValue);
		break;

	case ShaderParam::Type::Texture:
		tVar->SetResource(tValue->m_pTextureView);
		break;

	case ShaderParam::Type::Matrix:
		mVar->SetMatrix((float*)mValue);
		break;

	default:
		_assert(false);//unhandled case!
		break;
	}
}

// -----------------------------------------------------------------------------

ShaderParams::ShaderParams(
	Shader& shader
) : m_shader(shader)
{
}

// -----------------------------------------------------------------------------

void 
ShaderParams::Apply()
{
	for (auto iter = m_paramMap.begin(); iter != m_paramMap.end(); ++iter)
	{
		iter->second.Apply();
	}
}

// -----------------------------------------------------------------------------

void
ShaderParams::SetValue(
	const char*		name,
	ShaderParam&	param
)
{
	auto find = m_paramMap.find(name);
	if (find == m_paramMap.end())
	{
		param.SetVariable(m_shader.m_pEffect->GetVariableByName(name));

		m_paramMap.insert(std::make_pair(name, param));
	}
	else
	{
		param.SetVariable(find->second.variable);
		find->second = param;
	}
}

//----------------------------------------------------------------------------------------

void
GlobalShaderParams::Apply()
{
	for (auto iter = m_shaderList.begin(); iter != m_shaderList.end(); ++iter)
	{
		iter->second->Apply();
	}
}

//----------------------------------------------------------------------------------------

void
GlobalShaderParams::RegisterShader(
	Shader* shader
)
{
#ifdef _DEBUG
	for (auto iter = m_shaderList.begin(); iter != m_shaderList.end(); ++iter)
	{
		if (iter->first == shader)
		{
			// Already registered!
			_assert(false);
			return;
		}
	}
#endif

	//Register a new global shader params
	m_shaderList.push_back(std::make_pair(shader, std::unique_ptr<ShaderParams>(new ShaderParams(*shader))));
}

//----------------------------------------------------------------------------------------

}//namespace graphics