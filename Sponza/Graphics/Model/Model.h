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
#include "Graphics/Model/Model.h"
#include "Graphics/Model/Material.h"
#include "Graphics/Model/Mesh.h"
#include "Content/ContentItem.h"

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

class Model : public content::ContentItem
{
public:
	Model();
	~Model();

public:
	virtual void 
	Load(
		content::ContentReader& reader
	);

	void 
	Draw();

private:
	std::vector<Material>	m_materials;
	std::vector<Mesh>		m_meshes;

	static D3D10_INPUT_ELEMENT_DESC	s_layoutDesc[5];
};

// -----------------------------------------------------------------------------

}//namespace model
}//namespace graphics