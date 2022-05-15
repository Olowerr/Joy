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

	// fancy lightning

	return float4(image.Sample(defaultSampler, input.uv).rgb, 1.f);
}