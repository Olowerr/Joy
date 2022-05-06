cbuffer camMatrices : register(b0)
{
	float4x4 viewproject;
}

cbuffer camData : register(b1)
{
	float3 camPos;
	float pad;
}

struct output
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

output main(float3 pos : POSITION, float2 uv : UV)
{
	output o;
	o.uv = uv;
	//o.pos = mul(float4(pos + camPos, 1.f), viewproject).xyww;
	o.pos = float4(pos.xy, 1.f, 1.f);
	return o;
}