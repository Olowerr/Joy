struct input
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

Texture2D skyTexture : register(t0);
SamplerState sampy : register(s0);

float4 main(input vs_out) : SV_TARGET
{
	return float4(skyTexture.Sample(sampy, vs_out.uv).rgb, 0.f);
}