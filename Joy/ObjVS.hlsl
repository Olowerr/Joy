struct VS_IN
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float3 worldPos : WORLDPOS;
};

cbuffer world : register(b0)
{
	float4x4 world;
}

cbuffer cam : register(b1)
{
	float4x4 viewProj;
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	output.worldPos = mul(float4(input.pos, 1.0f), world);
	output.pos = mul(float4(output.worldPos, 1.0f), viewProj);
	output.normal = mul(float4(input.normal, 0.f), world).xyz;
	output.uv = input.uv;

	return output;
}