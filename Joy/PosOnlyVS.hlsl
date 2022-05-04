cbuffer world : register(b0)
{
	float4x4 worldMatrix;
}

cbuffer camera : register(b1)
{
	float4x4 viewProjectMatrix;
}

float4 main(float3 pos : POSITION) : SV_POSITION
{
	return mul(mul(float4(pos, 1.f), worldMatrix), viewProjectMatrix);
}