#include "stdafx.h"
#include "Camera.h"

// Initialize the view matrix
D3DXVECTOR3 Camera::m_Eye = D3DXVECTOR3( 0.0f, 30.0f, -200.0f );
D3DXVECTOR3 Camera::m_At = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
D3DXVECTOR3 Camera::m_Up = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

int Camera::m_mouseX = 0;
int Camera::m_mouseY = 0;
float Camera::m_rotX = 0.0f;
float Camera::m_rotY = 0.0f;
D3DXVECTOR3 Camera::m_Fwd = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
D3DXVECTOR3 Camera::m_Side = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
D3DXVECTOR2 Camera::m_Dir = D3DXVECTOR2( 0.0f, 0.0f );

Camera::Camera()
{
	float fAspect = static_cast<float>( 640.0f ) / static_cast<float>( 480.0f );
    D3DXMatrixPerspectiveFovLH( &m_Projection, D3DX_PI * 0.25f, fAspect, 100.0f, 3000.0f );
}

float lastDt = 0.0f;

void Camera::Update(float dt)
{
	lastDt = dt * 0.5f;

	m_Fwd.y = sinf( m_rotY );
	float dist = 1.0f - abs( m_Fwd.y );
	m_Fwd.x = sinf( m_rotX ) * dist;
	m_Fwd.z = cosf( m_rotX ) * dist;

	// Normalize look vector
	D3DXVec3Normalize( &m_Fwd, &m_Fwd );
	
	// Apply camera movement
	D3DXVec3Cross(&m_Side, &m_Fwd, &m_Up);
	m_Eye = m_Eye + (m_Fwd * m_Dir.y * dt) + (m_Side * m_Dir.x * dt);

	// Calculate final look at direction
	m_At = m_Eye + m_Fwd;

	D3DXMatrixLookAtLH( &m_View, &m_Eye, &m_At, &m_Up );
}

//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK Camera::OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	float value = bKeyDown ? 300.0f : 0.0f;

    switch( nChar )
    {
		case VK_UP:
			m_Dir.y = value;
            break;
		case VK_DOWN:
			m_Dir.y = -value;
			break;
		case VK_RIGHT:
			m_Dir.x = -value;
			break;
		case VK_LEFT:
			m_Dir.x = value;
			break;
    }
}

inline float Clamp(float value, float min, float max)
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}

//--------------------------------------------------------------------------------------
// Handle mouse presses
//--------------------------------------------------------------------------------------
void CALLBACK Camera::OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos, void* pUserContext )
{
	if (bLeftButtonDown)
	{
		int ddx = xPos - m_mouseX;
		int ddy = m_mouseY - yPos;

		m_rotX += (float)ddx * lastDt;
		m_rotY = Clamp(m_rotY + ((float)ddy * lastDt), -3.14f, 3.14f);
	}
	m_mouseX = xPos;
	m_mouseY = yPos;
}