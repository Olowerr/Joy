struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float3 worldPos : WORLDPOS;
};

float4 main(PS_IN input) : SV_TARGET
{

	float4 black = float4(0,0,0, 1);
/*uint2 textureDims;
lightMap.GetDimensions(textureDims.x, textureDims.y);
float lightValue = lightMap.Load(int3(input.uv * textureDims, 0)).r;*/

	return black;
}