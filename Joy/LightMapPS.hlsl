struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 wsPos : WS_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
};

cbuffer light : register(b0)
{
	float strength;
	float3 direction;
	float4x4 viewProject;
}

float4 main(PS_IN input) : SV_TARGET
{
	float dotty = dot(direction, input.normal);
	return float4(dotty, 0.f, 0.f, 0.f);
}