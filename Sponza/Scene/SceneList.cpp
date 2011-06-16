// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Scene/SceneList.h"
#include "Graphics/Shader.h"
#include "Graphics/Model/Model.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace scene
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

void SceneList::Draw(
	graphics::Shader* pShader
)
{
	//TODO: sorting + culling and drawing by pass (for lights, opaque, transparent materials)
	auto end = m_list.end();
	for (auto iter = m_list.begin(); iter != end; ++iter)
	{
		pShader->SetWorld((*iter)->m_world);
		(*iter)->m_pModel->Draw();
	}
}

// -----------------------------------------------------------------------------

void SceneList::Clear()
{
	m_list.clear();
}

// -----------------------------------------------------------------------------

void SceneList::DrawLights()
{
	m_lights.DrawLights();
}

// -----------------------------------------------------------------------------

Light* 
SceneList::CreateLight()
{
	return m_lights.CreateLight();
}

// -----------------------------------------------------------------------------

SceneInstWeak 
SceneList::Add(
	graphics::model::Model* pModel, 
	const D3DXVECTOR3& position
)
{
	SceneInstPtr inst = std::make_shared<SceneInst>(position, pModel);
	m_list.push_back(inst);
	return inst;
}

// -----------------------------------------------------------------------------

SceneInst::SceneInst(
	const D3DXVECTOR3& position,
	graphics::model::Model* pModel
) :
	m_pModel(pModel)
{
	SetPosition(position);
}

// -----------------------------------------------------------------------------

void SceneInst::SetPosition(const D3DXVECTOR3& position)
{
	D3DXMatrixTranslation(&m_world, position.x, position.y, position.z);
}

//----------------------------------------------------------------------------------------

}//namespace scene