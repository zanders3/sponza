#pragma once

#include "DXUT.h"

class Camera
{
private:
	static D3DXVECTOR3 m_Eye;
	static D3DXVECTOR3 m_At;
	static D3DXVECTOR3 m_Up;
public:
	D3DXMATRIX m_View, m_Projection;
	
	Camera();

	void Update(float dt);
	static void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
	static void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext );
};