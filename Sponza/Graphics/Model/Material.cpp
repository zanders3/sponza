// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/Model/Material.h"
#include "Graphics/Texture.h"
#include "Graphics/Shader.h"
#include "Content/ContentReader.h"
#include "Content/ContentManager.h"
#include "Graphics/Shader/ShaderParams.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------
namespace graphics
{
namespace model
{

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

Material::Material(
) : m_shader(nullptr)
{
}

// -----------------------------------------------------------------------------

Material::~Material()
{
}

// -----------------------------------------------------------------------------

Material::Material(
	Material&& other
)
{
	m_params.swap(other.m_params);
	m_shader = other.m_shader;
}

// -----------------------------------------------------------------------------

void 
Material::Load(
	content::ContentManager& manager,
	content::ContentReader& reader
)
{
	m_shader = manager.GetContent<Shader>("BlankShader.fx");
	m_params.reset(new ShaderParams(*m_shader));

	char* diffusePath = reader.ReadArray<char>();
	char* normalPath = reader.ReadArray<char>();

	m_params->SetValue("Diffuse", Texture::GetDiffuseDefault());
	m_params->SetValue("Normal", Texture::GetNormalDefault());

	if (*diffusePath)
	{
		manager.GetContentAsync<Texture>(
			diffusePath, 
			[=](content::ContentItem* item)
			{
				Texture* diffuse = dynamic_cast<Texture*>(item);
				m_params->SetValue("Diffuse", diffuse);
			});
	}
	if (*normalPath)
	{
		manager.GetContentAsync<Texture>(
			normalPath,
			[=](content::ContentItem* item)
			{
				Texture* normal = dynamic_cast<Texture*>(item);
				m_params->SetValue("Normal", normal);
			});
	}
}

//----------------------------------------------------------------------------------------

void 
Material::Bind()
{
	m_params->Apply();
	m_shader->Bind(0);
}

//----------------------------------------------------------------------------------------

}//namespace model
}//namespace graphics