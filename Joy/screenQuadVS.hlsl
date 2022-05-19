struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

VS_OUT main(float3 pos : POSITION)
{
	VS_OUT op;

	op.pos = float4(pos, 1.f);
	op.uv = float2(pos.x, pos.y * -1.f);
	op.uv = (op.uv + float2(1.f, 1.f)) * 0.5f;


	return op;
}