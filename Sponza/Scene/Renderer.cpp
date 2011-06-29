// -----------------------------------------------------------------------------
//	Copyright Alex Parker © 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Scene/Renderer.h"
#include "Scene/MeshQueue.h"
#include "Script/ScriptEngine.h"
#include "Script/ScriptType.h"
#include "Graphics/RenderTexture.h"
#include "Graphics/DepthTexture.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace scene
{

class RenderTextureHandle : public script::RefType<RenderTextureHandle>
{
public:
	static const char* TYPE_NAME;

	RenderTextureHandle(
	) : m_texture(nullptr)
	{
	}

	void Create(int width, int height)
	{
		assert(m_texture == nullptr);
		
	}

	void BindRT()
	{
		assert(m_texture != nullptr);
		m_texture->BindRT();
	}

	void Clear(float r, float g, float b, float a)
	{
		assert(m_texture == nullptr);
		m_texture->Clear(D3DXVECTOR4(r, g, b, a));
	}

private:
	graphics::RenderTexture* m_texture;
};

const char* RenderTextureHandle::TYPE_NAME = "RenderTexture";

static RenderTextureHandle* GetFramebuffer()
{
	return new RenderTextureHandle();
}

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

Renderer::Renderer(
	script::ScriptEngine&	scriptEngine, 
	MeshQueue&				meshQueue
) : m_meshQueue(meshQueue)
{
	RegisterType(scriptEngine, RenderTextureHandle);
	RegisterTypeMethod(scriptEngine, RenderTextureHandle, Create, "void Create(int, int)");
	RegisterTypeMethod(scriptEngine, RenderTextureHandle, BindRT, "void BindRT()");
	RegisterTypeMethod(scriptEngine, RenderTextureHandle, Clear, "void Clear(float,float,float,float)");

	RegisterGlobalFunction(scriptEngine, GetFramebuffer, "RenderTexture @GetFramebuffer()");
}

//----------------------------------------------------------------------------------------

Renderer::~Renderer()
{
}

//----------------------------------------------------------------------------------------

void
Renderer::Draw()
{

}

//----------------------------------------------------------------------------------------

}//namespace scene