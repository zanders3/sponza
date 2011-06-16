// -----------------------------------------------------------------------------
//	Copyright Alex Parker � 2011
//	
//	Model
//		- Loads and handles models and associated materials.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include <vector>
#include <memory>
#include "Scene/Lights.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace graphics
{
	namespace model
	{
		class Model;
	}
}

namespace scene
{
	class Light;
	class Shader;
	class SceneList;
	class SceneInst;

	typedef std::shared_ptr<SceneInst>	SceneInstPtr;
	typedef std::weak_ptr<SceneInst>	SceneInstWeak;
	typedef std::shared_ptr<SceneList>	SceneListPtr;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class SceneInst
{
public:
	friend SceneList;

	SceneInst(const D3DXVECTOR3& position, graphics::model::Model* pModel);
	void SetPosition(const D3DXVECTOR3& position);

private:
	//Only accessible by SceneList to do rendering
	D3DXMATRIX	m_world;
	graphics::model::Model*		m_pModel;
};

class SceneList
{
public:
	SceneInstWeak Add(graphics::model::Model* pModel, const D3DXVECTOR3& position);
	Light* CreateLight();

	void Draw(Shader* pShader);
	void DrawLights();

	void Clear();

private:
	std::vector<SceneInstPtr> m_list;
	LightList				  m_lights;
};

// -----------------------------------------------------------------------------

}//namespace scene