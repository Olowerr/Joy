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

	float lineThickness = 0.2;

	output.worldPos = mul(float4(input.pos, 1.0f), world).xyz;
	output.pos = mul(float4(output.worldPos, 1.0f), viewProj);
	output.normal = mul(float4(input.normal, 0.f), viewProj).xyz;
	float3 Normal = normalize(output.normal);
	output.uv = input.uv;
	output.pos = output.pos + float4(mul(0.06, Normal),0);


	

	return output;
}