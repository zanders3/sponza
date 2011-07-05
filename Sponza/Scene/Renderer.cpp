// -----------------------------------------------------------------------------
//	Copyright Alex Parker � 2011
// -----------------------------------------------------------------------------
#include "stdafx.h"

// -----------------------------------------------------------------------------
// Includes 
// -----------------------------------------------------------------------------
#include "Content/ContentManager.h"
#include "Scene/Renderer.h"
#include "Scene/MeshQueue.h"
#include "Script/Script.h"
#include "Script/ScriptEngine.h"
#include "Script/ScriptType.h"
#include "Graphics/RenderTexture.h"
#include "Graphics/DepthTexture.h"

// -----------------------------------------------------------------------------
// Namespace 
// -----------------------------------------------------------------------------

using namespace std;

namespace
{
	scene::Renderer* s_renderer = nullptr;
}

namespace scene
{

// -----------------------------------------------------------------------------

class RenderTextureHandle : public script::RefType<RenderTextureHandle>
{
public:
	static const char* TYPE_NAME;

	RenderTextureHandle()
	{
		assert(false);
	}

	RenderTextureHandle(
		graphics::RenderTexture* texture
	) : m_texture(texture)
	{
		assert(texture);
	}

	~RenderTextureHandle()
	{
		if (m_texture)
		{
			s_renderer->DeleteRenderTexture(m_texture);
		}
	}

	void BindRT()
	{
		m_texture->BindRT();
	}

	void Clear(float r, float g, float b, float a)
	{
		m_texture->Clear(D3DXVECTOR4(r, g, b, a));
	}

private:
	graphics::RenderTexture*	m_texture;
	bool						m_needsRelease;
};

// -----------------------------------------------------------------------------

const char* RenderTextureHandle::TYPE_NAME = "RenderTexture";

// -----------------------------------------------------------------------------

static RenderTextureHandle* GetFramebufferGlobal()
{
	assert(s_renderer);
	return new RenderTextureHandle(&s_renderer->GetFramebuffer());
}

// -----------------------------------------------------------------------------

static RenderTextureHandle* CreateRenderTexture(int width, int height)
{
	assert(s_renderer);
	return new RenderTextureHandle(&s_renderer->CreateRenderTexture(width, height));
}

// -----------------------------------------------------------------------------

static void DrawPassGlobal()
{
	assert(s_renderer);
	s_renderer->DrawPass();
}

// -----------------------------------------------------------------------------
// Class Implementation
// -----------------------------------------------------------------------------

Renderer::Renderer(
	script::ScriptEngine&	scriptEngine, 
	MeshQueue&				meshQueue
) : m_meshQueue(meshQueue),
	m_depthBuffer(),
	m_frameBuffer(),
	m_renderScript(nullptr)
{
	assert(s_renderer == nullptr);
	s_renderer = this;

	RegisterType(scriptEngine, RenderTextureHandle);
	RegisterTypeMethod(scriptEngine, RenderTextureHandle, BindRT, "void BindRT()");
	RegisterTypeMethod(scriptEngine, RenderTextureHandle, Clear, "void Clear(float,float,float,float)");

	RegisterGlobalFunction(scriptEngine, GetFramebufferGlobal, "RenderTexture @GetFramebuffer()");
	RegisterGlobalFunction(scriptEngine, DrawPassGlobal, "void DrawPass()");
}

//----------------------------------------------------------------------------------------

Renderer::~Renderer()
{
}

//----------------------------------------------------------------------------------------

void
Renderer::Draw()
{
	if (m_renderScript->CanInvoke())
	{
		m_renderScript->Invoke<void ()>("void Draw()")();
	}
}

//---------------------------------------------------------------------------------------

graphics::RenderTexture&
Renderer::CreateRenderTexture(int width, int height)
{
	m_renderTextureList.push_back(std::unique_ptr<graphics::RenderTexture>( new graphics::RenderTexture( m_depthBuffer.get(), DXUTGetDXGIBackBufferSurfaceDesc()->Format, width, height ) ));
	return *m_renderTextureList.back();
}

//---------------------------------------------------------------------------------------

void
Renderer::DeleteRenderTexture(graphics::RenderTexture* texture)
{
	if (m_renderTextureList.size() > 0)
	{
		for (auto iter = m_renderTextureList.begin(); iter != m_renderTextureList.end(); ++iter)
			if (iter->get() == texture)
			{
				m_renderTextureList.erase(iter);
				return;
			}
	}
}

//---------------------------------------------------------------------------------------

void
Renderer::LoadContent(
	content::ContentManager& contentManager
)
{
	m_depthBuffer.reset( new graphics::DepthTexture() );
	m_frameBuffer.reset( new graphics::RenderTexture(m_depthBuffer.get()) );
	
	m_renderScript = contentManager.GetContent<script::Script>("renderer.as");
}

//---------------------------------------------------------------------------------------

void
Renderer::DrawPass()
{
	m_meshQueue.Draw();
}

//---------------------------------------------------------------------------------------

}//namespace scene