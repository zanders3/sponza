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

private:
	std::vector<Material>	m_materials;
	std::vector<Mesh>		m_meshes;
};

// -----------------------------------------------------------------------------

}//namespace graphics