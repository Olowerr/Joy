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

float main(PS_IN input) : SV_TARGET
{
	return dot(-direction, input.normal);
}