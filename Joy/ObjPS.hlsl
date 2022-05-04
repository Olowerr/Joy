struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float3 worldPos : WORLDPOS;
};

Texture2D image : register(t0);
Texture2D lightMap : register(t2);

SamplerState defaultSampler : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
	float lightValue = lightMap.Sample(defaultSampler, input.uv).r;

	/*uint2 textureDims;
	lightMap.GetDimensions(textureDims.x, textureDims.y);
	float lightValue = lightMap.Load(int3(input.uv * textureDims, 0)).r;*/

	return image.Sample(defaultSampler, input.uv) * lightValue;
}