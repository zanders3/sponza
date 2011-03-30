#include "stdafx.h"
#include "Graphics/Texture.h"

using namespace std;

Texture::Texture() :
	m_pTexture(nullptr)
{
}

Texture::~Texture()
{
	SAFE_RELEASE(m_pTexture);
}

void Texture::Load(istream& input)
{
	//Read data into memory
	input.seekg(0, ios::end);
	int length = static_cast<int>(input.tellg());
	input.seekg(0, ios::beg);

	char * data = new char[length];
	input.read(data, length);

	//Create texture
	HRESULT hr;
	V(D3DX10CreateShaderResourceViewFromMemory(
		m_pDevice,
		data,
		length,
		NULL,
		NULL,
		&m_pTexture,
		NULL));

	delete[] data;
}

void Texture::Bind(const BindType& type)
{
	m_pDevice->PSSetShaderResources((UINT)type, 1, &m_pTexture);
}