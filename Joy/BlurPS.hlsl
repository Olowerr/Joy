Texture2D<unorm float4> blurData : register(t5);
SamplerState defaultSampler : register(s0);

float4 main(float4 pos : SV_POSITION, float2 uv : UV) : SV_TARGET
{
	return float4(blurData.Sample(defaultSampler, uv).a, 0.f, 0.f, 0.f);
}