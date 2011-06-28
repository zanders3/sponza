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

#include "Content/ContentItem.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

class asIScriptModule;
class asIScriptContext;

namespace script
{
// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

class Script : public content::ContentItem
{
public:
	friend class ScriptEngine;

	Script();
	~Script();

	virtual void 
	Load(content::ContentReader& reader);

private:
	int					m_updateFuncID;

	char*				m_script;
	u32					m_scriptSize;
	asIScriptContext*	m_context;
	asIScriptModule*	m_module;
};

// -----------------------------------------------------------------------------

}//namespace script