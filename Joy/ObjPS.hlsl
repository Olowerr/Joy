float4 main(float4 pos : SV_POSITION) : SV_TARGET
{
	float d = pos.z / pos.w;
	return float4(d, 0.0f, 0.0f, 1.0f);
}