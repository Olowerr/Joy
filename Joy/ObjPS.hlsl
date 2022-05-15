struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float3 worldPos : WORLDPOS;
	uint id : SV_InstanceID;
};

Texture2D image : register(t0);
Texture2DArray lightMap : register(t2);

SamplerState defaultSampler : register(s0);

static const float3 LightDir = normalize(float3(1.f, 1.f, -1.f));

float4 main(PS_IN input) : SV_TARGET
{
	const float lightValue = lightMap.Sample(defaultSampler, float3(input.uv, input.id)).r;

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

	intensity = clamp(intensity * lightValue, 0.2f, 1.f);

	/*uint2 textureDims;
	lightMap.GetDimensions(textureDims.x, textureDims.y);
	float lightValue = lightMap.Load(int3(input.uv * textureDims, 0)).r;*/

	return float4(image.Sample(defaultSampler, input.uv).rgb * intensity, 0.f);
}