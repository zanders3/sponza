#pragma once

#include "DXUT.h"

class Camera
{
private:
	static D3DXVECTOR3 m_Eye;
	static D3DXVECTOR3 m_At;
	static D3DXVECTOR3 m_Up;
	static int m_mouseX;
	static int m_mouseY;
	static float m_rotX;
	static float m_rotY;
	static D3DXVECTOR3 m_Fwd;
	static D3DXVECTOR3 m_Side;
	static D3DXVECTOR2 m_Dir;
public:
	D3DXMATRIX m_View, m_Projection;
	
	Camera();

	void Update(float dt);
	static void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
	static void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext );
};