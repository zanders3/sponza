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

namespace scene
{
	class SceneNode;
	typedef std::shared_ptr<SceneNode> SceneNodePtr;
}

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

	scene::SceneNodePtr&
	GetModelRoot();

private:
	scene::SceneNodePtr		m_rootNode;
	std::vector<Material>	m_materials;
};

// -----------------------------------------------------------------------------

}//namespace graphics