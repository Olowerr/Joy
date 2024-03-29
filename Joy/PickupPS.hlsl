struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

Texture2D image : register(t0);
SamplerState defaultSampler : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
    return image.Sample(defaultSampler, input.uv);
}