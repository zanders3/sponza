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

	void SetTransform(
		const D3DXMATRIX& transform
	);

	void SetPosition(
		const D3DXVECTOR3& position
	);

	void SetForward(
		const D3DXVECTOR3& forward
	);

	const D3DXMATRIX&
	GetTransform() const;

	const D3DXVECTOR3&
	GetPosition() const;

	const D3DXVECTOR3&
	GetForward() const;

private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_forward;
	D3DXMATRIX	m_transform;
	bool		m_dirty;
};

// -----------------------------------------------------------------------------

}//namespace scene