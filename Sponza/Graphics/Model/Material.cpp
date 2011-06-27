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
	static int count = 0;
	m_id = count++;
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
	m_id = other.m_id;
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
	m_params.reset(new ShaderParams(*m_shader));

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

bool
Material::operator< (
	const Material& other
) const
{
	return m_id < other.m_id;
}

//----------------------------------------------------------------------------------------

bool
Material::operator== (
	const Material& other
) const
{
	return m_id == other.m_id;
}

//----------------------------------------------------------------------------------------

bool
Material::operator!= (
	const Material& other
) const
{
	return m_id != other.m_id;
}

//----------------------------------------------------------------------------------------

}//namespace graphics