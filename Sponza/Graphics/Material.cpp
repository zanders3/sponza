#include "stdafx.h"
#include "Model.h"
#include "Content/ContentLoader.h"

using namespace std;

//----------------------------------------------------------------------------------------

void Material::Load(std::istream& input, ContentLoader* pLoader)
{
	ContentID::Type diffuse;
	input.read((char*)&diffuse, sizeof(size_t));

	ContentID::Type normal;
	input.read((char*)&normal, sizeof(size_t));

	mDiffuse = diffuse == 0 ? Texture::GetDiffuseDefault(pLoader->GetDevice()) : pLoader->Get<Texture>(diffuse);
	mNormal  = normal  == 0 ? Texture::GetNormalDefault(pLoader->GetDevice())  : pLoader->Get<Texture>(normal );
}

void Material::Bind()
{
	mDiffuse->Bind(Texture::Diffuse);
	mNormal->Bind(Texture::Normal);
}