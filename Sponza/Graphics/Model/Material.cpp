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

	mDiffuse = manager.Get<Texture>(std::string(diffusePath));
	mNormal  = manager.Get<Texture>(std::string(normalPath));
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