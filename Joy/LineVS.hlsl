cbuffer camera : register(b1)
{
	float4x4 viewProject;
}

float4 main( float3 pos : POSITION ) : SV_POSITION
{
	return mul(float4(pos, 1.f), viewProject);
}