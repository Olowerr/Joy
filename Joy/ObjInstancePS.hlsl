struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	uint ID : InstanceID;
};

Texture2DArray textures : register(t1);
SamplerState defaultSampler : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
	return textures.Sample(defaultSampler, float3(input.uv, input.ID));
}