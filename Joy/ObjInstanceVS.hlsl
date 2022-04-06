struct TransformData
{
	float4x4 worldMatrix;
};

StructuredBuffer<float4x4> worldMatrices : register(t0);

struct VS_OUT
{
	float4 pos : SV_POSITION;
	uint ID : SV_InstanceID;
};

cbuffer cam : register(b1)
{
	float4x4 viewProj;
}

VS_OUT main(float3 pos : POSITION, uint ID : SV_InstanceID)
{
	VS_OUT output;

	output.pos = mul(mul(float4(pos, 1.f), worldMatrices[ID]), viewProj);
	output.ID = ID;

	return output;
}