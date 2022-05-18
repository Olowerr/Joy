struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float3 worldPos : WORLDPOS;
};

Texture2D image : register(t0);
Texture2D glow : register(t1);
SamplerState defaultSampler : register(s0);

static const float3 LightDir = normalize(float3(1.f, 1.f, -1.f));

struct RTVS
{
	float4 mainBuffer : SV_TARGET0;
	float4 blurBuffer : SV_TARGET1;
};

RTVS main(PS_IN input)
{
	uint2 textureDims;
	image.GetDimensions(textureDims.x, textureDims.y);


	const float3 Normal = normalize(input.normal);

	float intensity = dot(normalize(LightDir), Normal);

	if (intensity > 0.5)
		intensity = 1.0f;
	else if (intensity > 0.4)
		intensity = 0.7f;
	else if (intensity > 0.3)
		intensity = 0.3f;
	else
		intensity = 0.2f;

	RTVS output;
	output.mainBuffer = image.Sample(defaultSampler, input.uv) * intensity;
	output.blurBuffer = glow.Sample(defaultSampler, input.uv);

	return output;
}