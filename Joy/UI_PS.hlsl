Texture2D image : register(t0);
SamplerState defaultSampler : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXTURECOORD) : SV_TARGET
{
	return image.Sample(defaultSampler, uv);
}