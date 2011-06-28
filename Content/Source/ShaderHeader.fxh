//--------------------------------------------------------------------------------------
// Global Constants
//--------------------------------------------------------------------------------------

shared cbuffer cbChangeOnResize
{
    matrix Projection;
	float2 ScreenSize = float2(640.0f, 480.0f);
	float  CameraFar = 3000.0f;
};

shared cbuffer cbChangesEveryFrame
{
	matrix View;
	float3 CameraPos;
	float3 CameraFwd;
};

cbuffer cbChangesEveryDraw
{
	matrix World;
};

Texture2D<float4> Diffuse;
Texture2D<float3> Normal;