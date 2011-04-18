#include "stdafx.h"
#include "Scene/SceneList.h"

void SceneList::Draw(Shader* pShader)
{
	//TODO: sorting + culling and drawing by pass (for lights, opaque, transparent materials)
	auto end = m_list.end();
	for (auto iter = m_list.begin(); iter != end; ++iter)
	{
		pShader->SetWorld((*iter)->m_world);
		(*iter)->m_pModel->Draw();
	}
}

void SceneList::Clear()
{
	m_list.clear();
}

void SceneList::DrawLights()
{
	m_lights.DrawLights();
}

Light* SceneList::CreateLight()
{
	return m_lights.CreateLight();
}

SceneInstWeak SceneList::Add(Model* pModel, const D3DXVECTOR3& position)
{
	SceneInstPtr inst = std::make_shared<SceneInst>(position, pModel);
	m_list.push_back(inst);
	return inst;
}

SceneInst::SceneInst(const D3DXVECTOR3& position, Model* pModel) :
	m_pModel(pModel)
{
	SetPosition(position);
}

void SceneInst::SetPosition(const D3DXVECTOR3& position)
{
	D3DXMatrixTranslation(&m_world, position.x, position.y, position.z);
}