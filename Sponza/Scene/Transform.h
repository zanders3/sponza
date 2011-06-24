// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Transform
//		- Represents a scene node transform.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace scene
{
// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class Transform
{
public:
	Transform();

	void SetMatrix(
		const D3DXMATRIX& matrix
	);

	void SetPosition(
		const D3DXVECTOR3& position
	);

	void SetForward(
		const D3DXVECTOR3& forward
	);

	const D3DXMATRIX&
	GetMatrix() const;

	const D3DXVECTOR3&
	GetPosition() const;

	const D3DXVECTOR3&
	GetForward() const;

private:
	D3DXVECTOR3			m_position;
	D3DXVECTOR3			m_forward;
	mutable D3DXMATRIX	m_transform;
	mutable bool		m_dirty;
};

// -----------------------------------------------------------------------------

}//namespace scene