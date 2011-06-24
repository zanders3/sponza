// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Scene/Transform.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace scene
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

Transform::Transform(
) : m_position(0.0f, 0.0f, 0.0f),
	m_forward(1.0f, 0.0f, 0.0f),
	m_dirty(true)
{
}

//---------------------------------------------------------------------------------------

void
Transform::SetMatrix(
	const D3DXMATRIX& matrix
)
{
	//No need to recalculate the matrix from the position and forward as we've just set it!
	m_dirty = false;
	m_transform = matrix;
}

//---------------------------------------------------------------------------------------

void
Transform::SetPosition(
	const D3DXVECTOR3& position
)
{
	m_dirty = true;
	m_position = position;
}

//---------------------------------------------------------------------------------------

void
Transform::SetForward(
	const D3DXVECTOR3& forward
)
{
	m_dirty = true;
	m_forward = forward;
}

//---------------------------------------------------------------------------------------

const D3DXMATRIX&
Transform::GetMatrix() const
{
	if (m_dirty)
	{
		m_dirty = false;

		D3DXMatrixTranslation(&m_transform, m_position.x, m_position.y, m_position.z);
		D3DXMATRIX rotation;
		D3DXMatrixRotationAxis(&rotation, &m_forward, 0.0f);
		D3DXMatrixMultiply(&m_transform, &m_transform, &rotation);
	}

	return m_transform;
}

//---------------------------------------------------------------------------------------

const D3DXVECTOR3&
Transform::GetPosition() const
{
	return m_position;
}

//---------------------------------------------------------------------------------------

const D3DXVECTOR3&
Transform::GetForward() const
{
	return m_forward;
}

//---------------------------------------------------------------------------------------

}//namespace scene