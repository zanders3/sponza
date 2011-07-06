#include "ShaderHeader.fxh"

Texture2D<float> Opacity;

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

NORMAL_PS_INPUT OpacityVS( NORMAL_VS_INPUT input )
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
// Opacity Pixel Shader
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
    float3 mapNormal = (Normal.Sample( samLinear, input.Tex ) * 2.0) - 1.0;
	float3 normal = TransformNormal(mapNormal, input.Normal, input.Tangent, input.BiNormal);
	normal.z = -normal.z;
	
	return float4(normal, input.Depth);
}

float4 OpacityPS( NORMAL_PS_INPUT input ) : SV_Target
{
	float4 color = pow(Diffuse.Sample( samLinear, input.Tex ), 2.0f);
	color.a = Opacity.Sample( samLinear, input.Tex );
	
	return color;
}


//--------------------------------------------------------------------------------------
// Rasterizer States
//--------------------------------------------------------------------------------------

BlendState AlphaBlend
{
	BlendEnable[0] = True;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};

BlendState NoBlend
{
	BlendEnable[0] = False;
};

RasterizerState FrontCull
{
	CullMode = Front;
};

//--------------------------------------------------------------------------------------
technique10 <int DrawOrder = 1;>
{
    pass Diffuse
    {
		SetRasterizerState(FrontCull);
		SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, OpacityVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, OpacityPS() ) );
    }
	
	pass Normal
	{
		SetBlendState(NoBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(FrontCull);
        SetVertexShader( CompileShader( vs_4_0, OpacityVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, NormalsPS() ) );
	}
}
