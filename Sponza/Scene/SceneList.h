#pragma once

#include "Graphics/Model.h"
#include "Graphics/Shader.h"

#include <vector>
#include <memory>

class SceneInst;
typedef std::shared_ptr<SceneInst> SceneInstPtr;
typedef std::weak_ptr<SceneInst> SceneInstWeak;

class SceneInst
{
public:
	SceneInst(const D3DXVECTOR3& position, Model* pModel);
	void SetPosition(const D3DXVECTOR3& position);

	D3DXMATRIX	m_world;
	Model*		m_pModel;
};

class SceneList;
typedef std::shared_ptr<SceneList> SceneListPtr;

class SceneList
{
public:
	SceneInstWeak Add(Model* pModel, const D3DXVECTOR3& position);
	void Draw(Shader* pShader);
	void Clear();

private:
	std::vector<SceneInstPtr> m_list;
};