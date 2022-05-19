struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 worldPos : WORLDPOS;
    uint id : SV_InstanceID;
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

static const float3 LightDir = normalize(float3(1.f, 1.f, -1.f));

float4 main(PS_IN input) : SV_TARGET
{
    //return float4(input.normal, 1.f);
    //return image.Sample(defaultSampler, input.uv);
    uint2 textureDims;
    image.GetDimensions(textureDims.x, textureDims.y);
    float lightValue = lightMap.Sample(defaultSampler, float3(input.uv, input.id)).r;
    
    float2 decal = float2(decalPosX, decalPosZ);
    float2 pixelXZ = float2(input.worldPos.x, input.worldPos.z);
    float2 isInside = decal - pixelXZ;
    float distance = (decalPosY) - (input.worldPos.y);
    float4 joyShadow = float4(0.05f, 0.05f, 0.05f, 0.05f);
    float4 shadowCamPos = mul(float4(input.worldPos, 1.0f), decalViewOrtMtrx);
    shadowCamPos.xyz /= shadowCamPos.w;
	
    float2 uvs = float2(shadowCamPos.x * 0.5f + 0.5f, shadowCamPos.y * -0.0f + 0.5f);
    float sampledUvs = decalDepth.Sample(defaultSampler, uvs).r;
	
    if (sampledUvs > shadowCamPos.z + 0.001f && shadowCamPos.z > -0.01 && (dot(float3(0.0f, 1.0f, 0.0f), input.normal) > 0.1f))
    {
        if (dot(isInside, isInside) < 0.5f - (distance * 0.04f) || dot(isInside, isInside) < 0.01f)
        {
            return float4(clamp((distance * 0.05f), joyShadow.rgb, image.Sample(defaultSampler, input.uv).rgb * (joyShadow.rgb + 0.5f)) * lightValue, 0.f);
        }
    }

    const float3 Normal = normalize(input.normal);

    float intensity = dot(normalize(LightDir), Normal);

    if (intensity > 0.5)
        intensity = 0.8f;
    else if (intensity > 0.4)
        intensity = 0.7f;
    else if (intensity > 0.3)
        intensity = 0.3f;
    else
        intensity = 0.2f;

    intensity = clamp(intensity * lightValue, 0.2f, 0.8f);

    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject
    // change from obj to joy in instancedObject

    //return float4(image.Sample(defaultSampler, input.uv).rgb * intensity, 0.f);
    return float4(image.Load(int3(input.uv * textureDims, 0)).rgb * intensity, 0.f);
}