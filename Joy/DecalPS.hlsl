struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 worldPos : WORLDPOS;
};

Texture2D image : register(t0);
Texture2D decalDepth : register(t1);
Texture2DArray lightMap : register(t2);
SamplerState defaultSampler : register(s0);


cbuffer decalShadow : register(b0)
{
    float decalPosX;
    float decalPosY;
    float decalPosZ;
    float decalPosW;
}

cbuffer decalCam : register(b1)
{
    float4x4 decalViewOrtMtrx;
}

float4 main(PS_IN input) : SV_TARGET
{
    float2 decal = float2(decalPosX, decalPosZ);
    float2 pixelXZ = float2(input.worldPos.x, input.worldPos.z);
    float2 isInside = decal - pixelXZ;
    float distance = (decalPosY) - (input.worldPos.y);
    float4 joyShadow = float4(0.05f, 0.05f, 0.05f, 0.05f);
    float4 shadowCamPos = mul(float4(input.worldPos, 1.0f), decalViewOrtMtrx);
    shadowCamPos.xyz /= shadowCamPos.w;
	
    float2 uvs = float2(shadowCamPos.x * 0.5f + 0.5f, shadowCamPos.y * -0.0f + 0.5f);
    float sampledUvs = decalDepth.Sample(defaultSampler, uvs).r;
	
    if (sampledUvs > shadowCamPos.z + 0.001f && shadowCamPos.z > 0 && (dot(float3(0.0f, 1.0f, 0.0f), input.normal) > 0.1f))
    {
        if (dot(isInside, isInside) < 1.0f - (distance * 0.2f) || dot(isInside, isInside) < 0.01f)
        {
            return clamp((distance * 0.05f), joyShadow, image.Sample(defaultSampler, input.uv) * (joyShadow + 0.5f)); //* lightMap.Sample(defaultSampler, input.uv).r;
        }
    }
    
    return image.Sample(defaultSampler, input.uv) * lightMap.Sample(defaultSampler, float3(input.uv, 0.f)).r;
}