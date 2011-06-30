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
	m_params = other.m_params;
	m_shader = other.m_shader;
}

// -----------------------------------------------------------------------------

void 
Material::Load(
	content::ContentManager& manager,
	content::ContentReader& reader
)
{
	char* diffusePath = reader.ReadArray<char>();
	char* normalPath = reader.ReadArray<char>();
	char* opacityPath = reader.ReadArray<char>();
	char* shader = reader.ReadArray<char>();

	m_shader = manager.GetContent<Shader>(shader);
	m_params = m_shader->CreateShaderParams();

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
	if (*opacityPath)
	{
		m_params->SetValue("Opacity", Texture::GetOpacityDefault());
		manager.GetContentAsync<Texture>(
			opacityPath,
			[=](content::ContentItem* item)
			{
				Texture* opacity = dynamic_cast<Texture*>(item);
				m_params->SetValue("Opacity", opacity);
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

void 
Material::SetWorld(
	D3DXMATRIX& world
)
{
	m_shader->SetWorld( world );
}

//----------------------------------------------------------------------------------------

Shader*
Material::GetShader()
{
	return m_shader;
}

//----------------------------------------------------------------------------------------

}//namespace graphics