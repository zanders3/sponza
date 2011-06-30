#include "ShaderHeader.fxh"

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//--------------------------------------------------------------------------------------
// Normals Vertex Shader
//--------------------------------------------------------------------------------------

struct NORMAL_VS_INPUT
{
    float4 Pos		: POSITION;
	float2 Tex		: TEXCOORD;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
	float3 BiNormal : BINORMAL;
};

struct NORMAL_PS_INPUT
{
    float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD0;
	float  Depth	: TEXCOORD1;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
	float3 BiNormal : BINORMAL;
};

NORMAL_PS_INPUT NormalsVS( NORMAL_VS_INPUT input )
{
    NORMAL_PS_INPUT output = (NORMAL_PS_INPUT)0;
	float4x4 wv = mul( World, View );
	float4 viewPos = mul( input.Pos, wv );
	output.Depth = viewPos.z / CameraFar;

	float4x4 wvp = mul( World, mul( View, Projection ) );
	output.Pos = mul( input.Pos, wvp );
	output.Tex = input.Tex;

	float3x3 worldT = (float3x3)World;
	output.Normal = mul(input.Normal, worldT);
	output.Tangent = mul(input.Tangent, worldT);
	output.BiNormal = mul(input.BiNormal, worldT);
    
    return output;
}

//--------------------------------------------------------------------------------------
// Normals Pixel Shader
//--------------------------------------------------------------------------------------

// Transforms Tangent Space Normal Maps into World Space.
float3 TransformNormal(float3 mapNormal, float3 surfNormal, float3 surfTangent, float3 surfBiNormal)
{
	float3x3 tangentBasis = float3x3(-surfTangent, surfBiNormal, -surfNormal);

	return normalize( mul( -mapNormal, tangentBasis ) );
}

float4 NormalsPS( NORMAL_PS_INPUT input ) : SV_Target
{
	//Apply normal mapping
    //float3 mapNormal = (Normal.Sample( samLinear, input.Tex ) * 2.0) - 1.0;
	//float3 normal = TransformNormal(mapNormal, input.Normal, input.Tangent, input.BiNormal);
	//normal.z = -normal.z;
	
	float4 color = Diffuse.Sample( samLinear, input.Tex ); //float4(normal, input.Depth);
	return pow(color, 2.0f);
}

//--------------------------------------------------------------------------------------
// Light Vertex Shader
// A light is a billboard that outputs the depth to make a circle shape.
//--------------------------------------------------------------------------------------

static const float PI = 3.14159265f;
static const int VertexCount = 8;
static const float Interval = (2.0f*PI)/VertexCount;

struct LIGHT_VERTEX_VS
{
    float4 Pos		: POSITION;
	float2 TexCoord	: TEXCOORD;
	float3 Color	: COLOR0;
	float  Size		: NORMAL;
};

struct LIGHT_VERTEX
{
    float4 Pos		: SV_POSITION;
	float3 TexCoord : TEXCOORD0;
	float3 LightPos : TEXCOORD1;	//Light position in world space
	float3 Color	: COLOR0;
};

LIGHT_VERTEX LightVS( LIGHT_VERTEX_VS input )
{
    LIGHT_VERTEX output = (LIGHT_VERTEX)0;
	
	float4x4 wv = mul( World, View );
	
	//View Space
	float4 viewPos = mul( input.Pos, wv );
	output.LightPos = viewPos.xyz;

	//Add billboard offset
	viewPos.xy += input.TexCoord.xy * input.Size;

	float4 front = viewPos;
	front.z += input.Size;

	//Projection Space
	output.Pos = mul( viewPos,	Projection );
	front	   = mul( front,	Projection );

	output.Color = input.Color;
	output.TexCoord.xy = input.TexCoord.xy*2.0f;

	output.TexCoord.z = (front.z/front.w) - (output.Pos.z/output.Pos.w);
    
    return output;
}

float4 LightPS( LIGHT_VERTEX input, out float depth : SV_DEPTH ) : SV_Target
{
	//Make a circle shape, and output depth to make the billboard a perfect sphere.
	float offset = sqrt(1.0f - (input.TexCoord.x*input.TexCoord.x) - (input.TexCoord.y*input.TexCoord.y));
	depth = input.Pos.z - (offset*input.TexCoord.z);

	//Sample the normal and depth from the Normals pass.
	float2 screenSpace = input.Pos.xy/ScreenSize;
	float4 surfaceNormal = Diffuse.Sample( samLinear, screenSpace );
	
	//Calculate the geometry surface position
	float3 surfacePos = CameraPos + surfaceNormal.w * CameraFwd;

	float3 lightDir = input.LightPos - surfacePos;
	float diffuse = dot(lightDir, surfaceNormal).x;

	return float4(diffuse.xxx, 1);
}

//--------------------------------------------------------------------------------------
// Fullscreen Quad
// Draws the final full colour pass to the screen.
//--------------------------------------------------------------------------------------

struct QUAD_INPUT
{
	float4 Pos		: POSITION;
	float2 Tex		: TEXCOORD;
};

struct QUAD_PS_INPUT
{
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD;
};

QUAD_PS_INPUT QuadVS( QUAD_INPUT input )
{
	QUAD_PS_INPUT output = (QUAD_PS_INPUT)0;
	output.Pos = input.Pos;
	output.Tex = input.Tex;
	return output;
}

float4 FinalPS( QUAD_PS_INPUT input ) : SV_Target
{
	return float4(Diffuse.Sample( samLinear, input.Tex ).xyz, 1.0f);
}

//--------------------------------------------------------------------------------------
// Rasterizer States
//--------------------------------------------------------------------------------------

BlendState NoBlend
{
	BlendEnable[0] = False;
};

RasterizerState FrontCull
{
	CullMode = Front;
};

RasterizerState NoCull
{
	CullMode = None;
};

RasterizerState BackCull
{
	CullMode = Back;
};

//--------------------------------------------------------------------------------------
technique10
{
    pass Diffuse
    {
		SetBlendState(NoBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(FrontCull);
        SetVertexShader( CompileShader( vs_4_0, NormalsVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, NormalsPS() ) );
    }

	pass Lights
	{
		SetRasterizerState(NoCull);
        SetVertexShader( CompileShader( vs_4_0, LightVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, LightPS() ) );
	}

	pass FinalPass
	{
		SetRasterizerState(NoCull);
		SetVertexShader( CompileShader( vs_4_0, QuadVS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, FinalPS() ) );
	}
}