//Renderer

RenderTexture@ framebuffer;

void Init()
{
	@framebuffer = GetFramebuffer();
}

void Draw()
{
	framebuffer.Clear( 0.0f, 0.3f, 1.0f, 1.0f );
	DrawPass();
}