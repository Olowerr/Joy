Texture2D image : register(t0);
SamplerState defaultSampler : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXTURECOORD) : SV_TARGET
{
	return float4(image.Sample(defaultSampler, uv).rgb, 0.f);
}