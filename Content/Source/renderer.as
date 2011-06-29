//Renderer

RenderTexture@ framebuffer;

void Init()
{
	print("Hello world!\n");
	@framebuffer = GetFramebuffer();
	framebuffer.Create(800, 600);
	//framebuffer.BindRT();
	//framebuffer.Clear();
}

void EmptyTest()
{
	print("Empty");
}

int RetInt()
{
	print("Return 5");
	return 5;
}

double One(double a)
{
	return a;
}

void MultiValue(int a, int b)
{
	print("multivalue");
}