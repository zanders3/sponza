//Renderer

RenderTexture@ framebuffer;
RenderTexture@ normalRT;

void Init()
{
	@framebuffer = GetFramebuffer();
	@normalRT = CreateRenderTexture();
}

void Draw()
{
	normalRT.BindRT();
	normalRT.Clear( 0.0f, 0.0f, 0.0f, 0.0f );
	DrawPass("Normal");
	
	framebuffer.BindRT();
	framebuffer.Clear( 0.0f, 0.3f, 1.0f, 1.0f );
	DrawPass("Diffuse");
}