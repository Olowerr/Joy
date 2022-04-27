struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 wsPos : WS_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
};

cbuffer light : register(b0)
{
	float lightStrength;
	float3 lightDirection;
	float4x4 lightViewProject;
}

Texture2D shadowMap : register(t0);
SamplerState defaultSampler : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
	float4 posLightSpace = mul(float4(input.wsPos, 1.f), lightViewProject);
	posLightSpace.xyz /= posLightSpace.w;
	float2 uvs = float2(posLightSpace.x * 0.5f + 0.5f, posLightSpace.y * -0.5f + 0.5f);

	float shadowMapDepth = shadowMap.Sample(defaultSampler, uvs).r;
	float shadowFactor = shadowMapDepth < posLightSpace.z;

	return float4(shadowFactor, 0.f, 0.f, 0.f);
}