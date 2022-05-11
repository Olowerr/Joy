StructuredBuffer<float4x4> worldMatrices : register(t0);

cbuffer cam : register(b1)
{
	float4x4 viewProj;
}

float4 main( float3 pos : POSITION, uint ID : SV_InstanceID ) : SV_POSITION
{
	return mul(mul(float4(pos, 1.f), worldMatrices[ID]), viewProj);
}