// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Graphics/Model/Material.h"
#include "Graphics/Texture.h"
#include "Content/ContentReader.h"
#include "Content/ContentManager.h"

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

void 
Material::Load(
	content::ContentManager& manager,
	content::ContentReader& reader
)
{
	char* diffusePath = reader.ReadArray<char>();
	char* normalPath = reader.ReadArray<char>();

	mDiffuse = Texture::GetDiffuseDefault();
	mNormal  = Texture::GetNormalDefault();

	if (*diffusePath)
	{
		manager.GetContentAsync<Texture>(
			diffusePath, 
			[=](content::ContentItem* item)
			{
				mDiffuse = dynamic_cast<Texture*>(item);
			});
	}
	if (*normalPath)
	{
		manager.GetContentAsync<Texture>(
			normalPath,
			[=](content::ContentItem* item)
			{
				mNormal = dynamic_cast<Texture*>(item);
			});
	}
}

//----------------------------------------------------------------------------------------

void 
Material::Bind()
{
	mDiffuse->Bind(Texture::Diffuse);
	mNormal->Bind(Texture::Normal);
}

//----------------------------------------------------------------------------------------

}//namespace model
}//namespace graphics