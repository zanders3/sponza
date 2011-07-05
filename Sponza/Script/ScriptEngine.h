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
	friend class ScriptContext;

	ScriptEngine();
	~ScriptEngine();

	void
	RegisterScript(
		Script* script
	);

	asIScriptEngine*
	GetEngine();

	static
	ScriptEngine&
	Instance();

private:
	void Execute(
		Script* script
	);

	std::vector<Script*>	m_scripts;
	asIScriptEngine*		m_engine;
};

// -----------------------------------------------------------------------------

}//namespace script