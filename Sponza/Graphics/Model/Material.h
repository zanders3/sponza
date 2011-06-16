// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Model
//		- Loads and handles models and associated materials.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

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

namespace model
{
	class Material;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class Material
{
public:
	void 
	Load(
		content::ContentManager&	manager,
		content::ContentReader&		reader
	);

	Texture* mDiffuse;
	Texture* mNormal;

	// Binds the textures to the device
	void Bind();
};

// -----------------------------------------------------------------------------

}//namespace model
}//namespace graphics