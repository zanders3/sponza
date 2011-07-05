// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Script/Script.h"
#include "Script/ScriptEngine.h"
#include "angelscript.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace script
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------
	
ScriptContext::ScriptContext(
	const char* name,
	Script&		script
) : m_script(script),
	m_funcId(script.m_module->GetFunctionIdByDecl(name))
{
}

// -----------------------------------------------------------------------------

void 
ScriptContext::Prepare()
{
	m_script.m_context->Prepare(m_funcId);
}

// -----------------------------------------------------------------------------

void 
ScriptContext::Execute()
{
	m_script.m_engine->Execute(&m_script);
}

// -----------------------------------------------------------------------------

asIScriptContext* 
ScriptContext::operator->(
) const
{
	return m_script.m_context;
}

// -----------------------------------------------------------------------------

Script::Script(
) : m_script(nullptr),
	m_scriptSize(0),
	m_context(nullptr),
	m_module(nullptr),
	m_engine(nullptr)
{
}

// -----------------------------------------------------------------------------

Script::~Script()
{
	SAFE_RELEASE(m_context);
}

// -----------------------------------------------------------------------------

void 
Script::Load(
	content::ContentReader& reader
)
{
	m_scriptSize = reader.Size();
	m_script = reader.Read<char>(m_scriptSize);

	ScriptEngine::Instance().RegisterScript(this);
}

// -----------------------------------------------------------------------------

}//namespace script