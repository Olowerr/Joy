struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float3 worldPos : WORLDPOS;
};

Texture2D image : register(t0);
Texture2D lightMap : register(t2);

SamplerState defaultSampler : register(s0);

cbuffer decalShadow : register(b0)
{
	float decalPosX;
	float decalPosY;
	float decalPosZ;
	float decalPosW;
}

float4 main(PS_IN input) : SV_TARGET
{
	float2 decal = float2(decalPosX, decalPosZ);
	float2 pixelXZ = float2(input.worldPos.x, input.worldPos.z);
	float2 isInside = decal - pixelXZ;
	float distance = (decalPosY) - (input.worldPos.y);
    float4 joyShadow = float4(0.05f, 0.05f, 0.05f, 0.05f);

	if (dot(isInside, isInside) < 1.0f - (distance * 0.2f) || dot(isInside, isInside) < 0.01f)
	{
        return clamp((distance * 0.05f), joyShadow, image.Sample(defaultSampler, input.uv) * (joyShadow + 0.5f));
    }
	
	float lightValue = lightMap.Sample(defaultSampler, input.uv).r;

	/*uint2 textureDims;
	lightMap.GetDimensions(textureDims.x, textureDims.y);
	float lightValue = lightMap.Load(int3(input.uv * textureDims, 0)).r;*/

	return image.Sample(defaultSampler, input.uv) * lightValue;
}