// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Script/ScriptEngine.h"
#include "Script/scriptstdstring.h"
#include <string>
#include "angelscript.h"
#include "Script/Script.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace
{
	void MessageCallback(const asSMessageInfo *msg, void *param)
	{
		const char *type = "ERR ";
		if( msg->type == asMSGTYPE_WARNING ) 
		type = "WARN";
		else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "INFO";
		char buffer[255];
		sprintf_s<255>(buffer, "%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
		OutputDebugStringA(buffer);
	}

	void Print(const std::string& msg)
	{
		OutputDebugStringA(msg.c_str());
	}
}

namespace script
{

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

ScriptEngine::ScriptEngine()
{
	HRESULT hr;

	m_engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);

	V(m_engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL));

	RegisterStdString(m_engine);

	V(m_engine->RegisterGlobalFunction("void print(const string& in)", asFUNCTION(Print), asCALL_CDECL));
}

// -----------------------------------------------------------------------------

ScriptEngine::~ScriptEngine()
{
	m_engine->Release();
}

// -----------------------------------------------------------------------------

void
ScriptEngine::RegisterScript(
	Script* script
)
{
	if (!script->m_module)
	{
		script->m_module = m_engine->GetModule(script->m_script, asGM_ALWAYS_CREATE);

		// Build the actual script
		m_engine->SetEngineProperty(asEP_COPY_SCRIPT_SECTIONS, true);
		script->m_module->AddScriptSection("MyScript", script->m_script, script->m_scriptSize);

		HRESULT hr;
		V(script->m_module->Build());

		// Call the script's Init() function
		int funcID = script->m_module->GetFunctionIdByDecl("void Init()");
		_assert(funcID >= 0);
		script->m_updateFuncID = script->m_module->GetFunctionIdByDecl("void Update(float secs)");
		_assert(script->m_updateFuncID >= 0);

		script->m_context = m_engine->CreateContext();
		
		V(script->m_context->Prepare(funcID));
		Execute(script);

		m_scripts.push_back(script);
	}
}

// -----------------------------------------------------------------------------

void
ScriptEngine::Update(
	float secs
)
{
	for (auto iter = m_scripts.begin(); iter != m_scripts.end(); ++iter)
	{
		Script* script = *iter;
		script->m_context->Prepare(script->m_updateFuncID);
		script->m_context->SetArgFloat(0, secs);
		
		Execute(script);
	}
}

// -----------------------------------------------------------------------------

asIScriptEngine*
ScriptEngine::GetEngine()
{
	return m_engine;
}

// -----------------------------------------------------------------------------

void 
ScriptEngine::Execute(
	Script* script
)
{
	int r = script->m_context->Execute();

	if ( r == asEXECUTION_EXCEPTION )
	{
		int column;
		const char* sectionName;
		int line = script->m_context->GetExceptionLineNumber(&column, &sectionName);

		char buffer[255];
		sprintf_s<255>(buffer, "An exception '%s' occurred. Please correct the code and try again.\n%s:%d,%d", script->m_context->GetExceptionString(), sectionName, line, column);
		OutputDebugStringA(buffer);
	}
}

// -----------------------------------------------------------------------------

}//namespace script