struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float3 worldPos : WORLDPOS;
};

Texture2D image : register(t0);
SamplerState defaultSampler : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
	uint2 textureDims;
	image.GetDimensions(textureDims.x, textureDims.y);

	// fancy lightning

	return float4(image.Load(int3(input.uv * textureDims, 0)).rgb, 1.f);
}