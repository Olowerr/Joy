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

float4 main(PS_IN input) : SV_TARGET
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

	float4 final;
	final.rgb = image.Sample(defaultSampler, input.uv).rgb * intensity;
	final.a = glow.Sample(defaultSampler, input.uv).r;

	return final;
}