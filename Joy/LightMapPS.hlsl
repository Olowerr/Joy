struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 wsPos : WS_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
};

cbuffer light : register(b0)
{
	float lightStrength;
	float3 lightDirection;
}
cbuffer lightMatrix : register(b1)
{
	float4x4 lightViewProject;
}

Texture2D shadowMap : register(t0);
SamplerState defaultSampler : register(s0);

#define xyRadius 2
#define texelSampleDist 1.f

float GetLightLevel(const float2 uvs, const float lightSpaceZ)
{
	float2 dimensions;
	shadowMap.GetDimensions(dimensions.x, dimensions.y);
	float2 texelSize = texelSampleDist / dimensions;

	float lightValue = 0.f;
	for (int x = -xyRadius; x <= xyRadius; x++)
	{
		for (int y = -xyRadius; y <= xyRadius; y++)
		{
			float shadowMapDepth = shadowMap.Sample(defaultSampler, uvs + texelSize * float2(x, y)).r;
			lightValue += shadowMapDepth + 0.001f > lightSpaceZ;
		}
	}

	return clamp(lightValue / float(pow(xyRadius * 2 + 1, 2)), 0.2f, 1.f);
}

float4 main(PS_IN input) : SV_TARGET
{
	//return float4(1.f, 0.f, 0.f, 0.f);

	float4 posLightSpace = mul(float4(input.wsPos, 1.f), lightViewProject);
	posLightSpace.xyz /= posLightSpace.w;
	float2 uvs = float2(posLightSpace.x * 0.5f + 0.5f, posLightSpace.y * -0.5f + 0.5f);

	float3 Normal = normalize(input.normal);
	
	float shadowFactor = GetLightLevel(uvs, posLightSpace.z);
	float intensity = dot(normalize(lightDirection), Normal);

	if (intensity < 0)
		intensity = 0;

	if (intensity > 0.7)
		intensity = 1.0f;
	else if (intensity > 0.5)
		intensity = 0.7f;
	else if (intensity > 0.3)
		intensity = 0.3f;
	else
		intensity = 0.2f;

	return float4(clamp(shadowFactor * intensity, 0.2f, 1.f), 0.f, 0.f, 0.f);
}