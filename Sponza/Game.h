#pragma once

#include "resource.h"
#include "Content\ContentLoader.h"
#include "Graphics\Model.h"
#include "Graphics\Shader.h"
#include "Graphics\Camera.h"

class Game
{
public:
	Game();
	~Game();

	void LoadContent( ID3D10Device* pd3dDevice, int width, int height );
	void Render( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime );
	void Update( double fTime, float fElapsedTime ); 

private:
	ContentLoader m_content;
	Camera		  m_camera;

	Model*		  m_model;
	Shader*		  m_shader;
};