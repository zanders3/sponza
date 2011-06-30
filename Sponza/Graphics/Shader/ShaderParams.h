// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	ShaderParams
//		- Stores and sets Shader Parameters
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"
#include <unordered_map>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace graphics
{
	class Texture;
	class Shader;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

struct ShaderParam
{
	struct Type
	{
		enum Enum
		{
			Float,
			Vector,
			Int,
			Texture,
			Matrix
		};
	};

	explicit ShaderParam(float value);
	explicit ShaderParam(D3DXVECTOR2& value);
	explicit ShaderParam(D3DXVECTOR3& value);
	explicit ShaderParam(D3DXVECTOR4& value);
	explicit ShaderParam(int value);
	explicit ShaderParam(Texture* value);
	explicit ShaderParam(D3DMATRIX* value);

	Type::Enum				type;
	ID3D10EffectVariable*	variable;
	union
	{
		float		fValue[4];
		int			iValue;
		Texture*	tValue;
		D3DMATRIX*	mValue;
	};
	union
	{
		ID3D10EffectScalarVariable*			sVar;
		ID3D10EffectVectorVariable*			vVar;
		ID3D10EffectShaderResourceVariable*	tVar;
		ID3D10EffectMatrixVariable*			mVar;
	};

	void SetVariable(ID3D10EffectVariable* var);
	void Apply();
};

// -----------------------------------------------------------------------------

class ShaderParams
{
	friend class Shader;

private:
	ShaderParams(
		Shader& shader
	);

public:

	void 
	Apply();

	template <typename T> void
	SetValue(
		const char*	name,
		const T&	value
	)
	{
		ShaderParam param(value);
		SetValue(name, param);
	}

private:
	void 
	SetValue(
		const char*		name,
		ShaderParam&	param
	);

	Shader&												m_shader;
	std::unordered_map<std::string, ShaderParam>		m_paramMap;
};

// -----------------------------------------------------------------------------

class GlobalShaderParams
{
public:
	friend class Shader;

private:
	static void
	RegisterShader(
		Shader* shader
	);

public:
	static void
	Apply();

	template <typename T> static void
	SetValue(
		const char*	name,
		const T&	value
	)
	{
		for (auto iter = m_shaderList.begin(); iter != m_shaderList.end(); ++iter)
		{
			iter->second->SetValue<T>(name, value);
		}
	}

private:
	static std::vector<std::pair<Shader*, ShaderParams*>>	m_shaderList;
};

// -----------------------------------------------------------------------------

}//namespace graphics