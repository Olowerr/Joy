struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

StructuredBuffer<float4x4> worldMatrices : register(t0);

cbuffer cam : register(b1)
{
    float4x4 viewProj;
}

VS_OUT main(VS_IN input, uint ID : SV_InstanceID)
{
    VS_OUT output;

    output.pos = mul(mul(float4(input.pos, 1.f), worldMatrices[ID]), viewProj);
    output.normal = mul(float4(input.normal, 0.f), worldMatrices[ID]).xyz;
    output.uv = input.uv;

    return output;
}