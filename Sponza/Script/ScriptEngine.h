// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	Texture
//		- Base class for all textures!
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include <vector>

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

class asIScriptEngine;

namespace script
{
	class Script;

// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class ScriptEngine
{
public:
	ScriptEngine();
	~ScriptEngine();

	void
	RegisterScript(
		Script* script
	);

	void
	Update(
		float secs
	);

	asIScriptEngine*
	GetEngine();

private:
	void Execute(
		Script* script
	);

	std::vector<Script*>	m_scripts;
	asIScriptEngine*		m_engine;
};

// -----------------------------------------------------------------------------

}//namespace script