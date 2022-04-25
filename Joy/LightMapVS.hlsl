struct VS_IN
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float3 wsPos : WS_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
};

cbuffer cam : register(b1)
{
	float4x4 viewProj;
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	output.pos = mul(float4(input.uv, 0.f, 1.f), viewProj);
	output.wsPos = mul(float4(input.pos, 1.f), world);
	output.normal = mul(float4(input.normal, 0.f), world).xyz;
	output.uv = input.uv;

	return output;
}