Texture2D<unorm float> blurData : register(t5);
SamplerState defaultSampler : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : UV) : SV_TARGET
{
	return blurData.Sample(defaultSampler, uv);
}