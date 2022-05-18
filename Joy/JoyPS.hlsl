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

float4 main(PS_IN input) : SV_TARGET
{
	uint2 textureDims;
	image.GetDimensions(textureDims.x, textureDims.y);

	// fancy lightning

	float4 final;
	final.rgb = image.Sample(defaultSampler, input.uv).rgb;
	final.a = glow.Sample(defaultSampler, input.uv).a;

	return final;
}