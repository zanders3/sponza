#pragma once

#include "Graphics/Model.h"
#include "Graphics/Shader.h"
#include "Graphics/Lights.h"

#include <vector>
#include <memory>

class SceneList;
class SceneInst;

typedef std::shared_ptr<SceneInst>	SceneInstPtr;
typedef std::weak_ptr<SceneInst>	SceneInstWeak;
typedef std::shared_ptr<SceneList>	SceneListPtr;

class SceneInst
{
public:
	friend SceneList;

	SceneInst(const D3DXVECTOR3& position, Model* pModel);
	void SetPosition(const D3DXVECTOR3& position);

private:
	//Only accessible by SceneList to do rendering
	D3DXMATRIX	m_world;
	Model*		m_pModel;
};

class SceneList
{
public:
	SceneInstWeak Add(Model* pModel, const D3DXVECTOR3& position);
	Light* CreateLight();

	void Draw(Shader* pShader);
	void DrawLights();

	void Clear();

private:
	std::vector<SceneInstPtr> m_list;
	LightList				  m_lights;
};