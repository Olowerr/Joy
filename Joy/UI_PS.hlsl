Texture2D image : register(t0);
SamplerState defaultSampler : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : TEXTURECOORD) : SV_TARGET
{
	float4 color = image.Sample(defaultSampler, uv);

	if (color.a == 0)
	{
		discard;
	}

	return float4(color.rgb, 0.f);
}