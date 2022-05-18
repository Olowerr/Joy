Texture2D mainBuffer : register(t5);
Texture2D<unorm float> blurData : register(t6);
SamplerState defaultSampler : register(s0);

static const float3 blurColour = float3(0.43137f, 0.96471f, 1.f);

float4 main(float4 pos : SV_POSITION, float2 uv : UV) : SV_TARGET
{
	float2 dimensions;
	mainBuffer.GetDimensions(dimensions.x, dimensions.y);

	float4 texelColour = mainBuffer.Load(int3(uv * dimensions, 0));
	float blurStrength = blurData.Sample(defaultSampler, uv);

	float4 final;
	final.r = blurStrength * blurColour.r;
	final.g = blurStrength * blurColour.g;
	final.b = blurStrength * blurColour.b;

	final.rgb += texelColour.rgb;
	final.a = 0.f;

	//if (texelColour.a > 0.f)
	//	return texelColour;

	return final;
}