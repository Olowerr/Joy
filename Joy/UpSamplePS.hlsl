Texture2D mainBuffer : register(t5);
Texture2D<unorm float> blurData : register(t6);
SamplerState defaultSampler : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : UV) : SV_TARGET
{
	float2 dimensions;
	mainBuffer.GetDimensions(dimensions.x, dimensions.y);

	float4 texelColour = mainBuffer.Load(int3(uv * dimensions, 0));
	float blurStrength = blurData.Sample(defaultSampler, uv);

	float4 final;
	final.x = texelColour.x + blurStrength;
	final.y = texelColour.y + blurStrength;
	final.z = texelColour.z + blurStrength;
	final.a = 0.f;

	if (texelColour.a > 0.f)
		return texelColour;

	return final;
}