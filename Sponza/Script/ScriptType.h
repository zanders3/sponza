// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
//	
//	RefType
//		- Template to help register a script reference type.
// -----------------------------------------------------------------------------

#pragma once

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "stdafx.h"

#include "angelscript.h"
#include "Script/ScriptEngine.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace script
{
// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

/*template <typename Method> 
static void RegisterGlobalMethod(
	const char*			methodSignature,
	asIScriptEngine*	engine
)
{
	int r = engine->RegisterGlobalMethod(m_typeName, methodSignature, asMETHOD(T, Method), asCALL_THISCALL); assert(r >= 0);
}*/

// -----------------------------------------------------------------------------

template <class T> class RefType
{
public:
	RefType(
	) : m_refCount(1)
	{
	}

	virtual ~RefType() {}

	void AddRef()
	{
		++m_refCount;
	}

	void Release()
	{
		if (--m_refCount == 0)
			delete this;
	}

	static T* Factory()
	{
		return new T();
	}

	//Use RegisterType() instead!
	static void Register(
		ScriptEngine&		scriptEngine
	)
	{
		const char* typeName = T::TYPE_NAME;
		asIScriptEngine* engine = scriptEngine.GetEngine();

		int r;
		r = engine->RegisterObjectType(typeName, 0, asOBJ_REF); assert(r >= 0);

		std::string factoryDef = typeName;
		factoryDef += "@ f()";
		r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_FACTORY, factoryDef.c_str(), asFUNCTION(T::Factory), asCALL_CDECL); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_ADDREF, "void f()", asMETHOD(T, AddRef), asCALL_THISCALL); assert( r >= 0 );
		r = engine->RegisterObjectBehaviour(typeName, asBEHAVE_RELEASE, "void f()", asMETHOD(T, Release), asCALL_THISCALL); assert( r >= 0 );
	}

private:
	u32	m_refCount;
};

// -----------------------------------------------------------------------------

#define RegisterType(scriptEngine, MyRefType) { MyRefType::Register(scriptEngine); }
#define RegisterTypeMethod(scriptEngine, MyRefType, MyFunction, Prototype) { int r = scriptEngine.GetEngine()->RegisterObjectMethod(MyRefType::TYPE_NAME, Prototype, asMETHOD(MyRefType, MyFunction), asCALL_THISCALL); assert(r >= 0); }

// -----------------------------------------------------------------------------

#define RegisterGlobalFunction(scriptEngine, MyFunction, Prototype) { int r = scriptEngine.GetEngine()->RegisterGlobalFunction( Prototype, asFUNCTION(MyFunction), asCALL_CDECL); assert(r >= 0); }

}//namespace script