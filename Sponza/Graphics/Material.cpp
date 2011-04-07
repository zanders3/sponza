#include "stdafx.h"
#include "Model.h"
#include "Content/ContentLoader.h"

using namespace std;

//----------------------------------------------------------------------------------------

Texture* Material::GetDiffuseDefault(ID3D10Device* pDevice)
{
	static Texture texture(pDevice, D3DCOLOR_ARGB(255, 255, 0, 255));

	return &texture;
}

Texture* Material::GetNormalDefault(ID3D10Device* pDevice)
{
	static Texture texture(pDevice, D3DCOLOR_RGBA(255, 128, 128, 255));

	return &texture;
}

//----------------------------------------------------------------------------------------

void Material::Load(std::istream& input, ContentLoader* pLoader)
{
	ContentID::Type diffuse;
	input.read((char*)&diffuse, sizeof(size_t));

	ContentID::Type normal;
	input.read((char*)&normal, sizeof(size_t));

	mDiffuse = diffuse == 0 ? GetDiffuseDefault(pLoader->GetDevice()) : pLoader->Get<Texture>(diffuse);
	mNormal  = normal  == 0 ? GetNormalDefault(pLoader->GetDevice())  : pLoader->Get<Texture>(normal );
}

void Material::Bind()
{
	mDiffuse->Bind(Texture::Diffuse);
	mNormal->Bind(Texture::Normal);
}