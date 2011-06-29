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
#include "angelscript.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

namespace script
{
	class ScriptEngine;
// -----------------------------------------------------------------------------
// Class Definition 
// -----------------------------------------------------------------------------

template <typename F> class ScriptFunction;

class Script;

class ScriptContext
{
public:
	ScriptContext(
		const char* name,
		Script&		script
	);

	void Prepare();
	void Execute();

	asIScriptContext* 
	operator->(
	) const;

private:
	Script& m_script;
	u32		m_funcId;
};

// -----------------------------------------------------------------------------

class Script : public content::ContentItem
{
public:
	friend class ScriptContext;
	friend class ScriptEngine;

	Script();
	~Script();

	virtual void 
	Load(content::ContentReader& reader);

	template <typename F>
	ScriptFunction<F> Invoke(const char* decl)
	{
		return ScriptFunction<F>(ScriptContext(decl, *this));
	}

private:
	char*				m_script;
	u32					m_scriptSize;
	asIScriptContext*	m_context;
	ScriptEngine*		m_engine;
	asIScriptModule*	m_module;
};

// -----------------------------------------------------------------------------

template <typename T> struct ScriptSetArgValue
{
	void operator()(
		const ScriptContext&	context,  
		int						arg, 
		T&						value
	) const
    {  
		context->SetArgObject(arg, &value);
	}
};

// -----------------------------------------------------------------------------

template <typename T> struct ScriptSetArgValue<T*>
{
	void operator()(
		const ScriptContext&	context,  
		int						arg, 
		T*						value
	) const  
    {  
		context->SetArgObject(arg, value);
	}
};

// -----------------------------------------------------------------------------

template <> struct ScriptSetArgValue<double>
{
	void operator()(
		const ScriptContext&	context,  
		int						arg, 
		double					value
	) const  
    {  
		context->SetArgDouble(arg, value);
	}
};

// -----------------------------------------------------------------------------

template <> struct ScriptSetArgValue<int>
{
	void operator()(
		const ScriptContext&	context,  
		int						arg, 
		int						value
	) const  
    {  
		context->SetArgDWord(arg, value);
	}
};

// -----------------------------------------------------------------------------

template <typename T> struct ScriptGetReturnValue  
{  
    T* operator()(const ScriptContext& context) const  
    {  
		return static_cast<T*>(context->GetReturnObject());
    }
};   
 
// -----------------------------------------------------------------------------

template <> struct ScriptGetReturnValue<double>  
{  
    double operator()(  
        const ScriptContext& context) const  
    {  
		return context->GetReturnDouble();  
    }  
};  
 
// -----------------------------------------------------------------------------

template <> struct ScriptGetReturnValue<int>  
{  
    int operator()(ScriptContext& context) const  
    {  
		return context->GetReturnDWord();  
    }  
};

// -----------------------------------------------------------------------------

template <> struct ScriptGetReturnValue<void>  
{  
    void operator()(const ScriptContext& context) const  
    {
    }  
};

// -----------------------------------------------------------------------------

template <typename R>  
class ScriptFunction<R ()>  
{  
private:  
    ScriptContext& m_context;  
 
public:  
    explicit ScriptFunction(ScriptContext& context)  
    :  m_context(context)  
    {}  
 
    R operator()()  
    {
		m_context.Prepare();
		m_context.Execute();

		return ScriptGetReturnValue<R>()(m_context);
    }
};

// -----------------------------------------------------------------------------

template <typename R, typename Arg0>  
class ScriptFunction<R (Arg0)>  
{  
private:  
    ScriptContext& m_context;  
 
public:  
    explicit ScriptFunction(ScriptContext& context)  
    :  m_context(context)  
    {}  
 
    R operator()(Arg0 value0)  
    {
		m_context.Prepare();  
 
		ScriptSetArgValue<Arg0>()(m_context, 0, value0);
 
		m_context.Execute();

		return ScriptGetReturnValue<R>()(m_context);
    }
};

// -----------------------------------------------------------------------------

template <typename R, typename Arg0, typename Arg1>  
class ScriptFunction<R (Arg0,Arg1)>  
{  
private:  
    ScriptContext& m_context;  
 
public:  
    explicit ScriptFunction(ScriptContext& context)  
    :  m_context(context)  
    {}  
 
    R operator()(Arg0 value0, Arg1 value1)  
    {
		m_context.Prepare();  
 
		ScriptSetArgValue<Arg0>()(m_context, 0, value0);  
		ScriptSetArgValue<Arg1>()(m_context, 1, value1);  
 
		m_context.Execute();

		return ScriptGetReturnValue<R>()(m_context);  
    } 
};

// -----------------------------------------------------------------------------

}//namespace script