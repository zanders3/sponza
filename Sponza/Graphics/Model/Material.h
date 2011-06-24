// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Material
//		- Handles material values, shaders and textures.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"
#include <memory>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace content
{
	class ContentManager;
	class ContentReader;
}

namespace graphics
{
	class Texture;
	class ShaderParams;
	class Shader;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class Material
{
public:
	Material();
	~Material();
	Material(Material&& other);

	void 
	Load(
		content::ContentManager&	manager,
		content::ContentReader&		reader
	);

	//Binds the current material
	void Bind();

	//Sets the world outside of the parameter bind.
	void SetWorld(
		D3DXMATRIX& world
	);

	bool
	operator< (
		const Material& other
	) const;

	bool
	operator= (
		const Material& other
	) const;

private:
	Material(const Material& other);

	int								m_id;
	std::unique_ptr<ShaderParams>	m_params;
	Shader*							m_shader;
};

// -----------------------------------------------------------------------------

}//namespace graphics