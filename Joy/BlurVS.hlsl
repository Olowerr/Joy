struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

VS_OUT main(float4 pos : SV_POSITION)
{
	VS_OUT op;

	op.pos = pos;
	op.uv = float2(pos.x, pos.y * -1.f);
	op.uv = (op.uv + float2(1.f, 1.f)) * 0.5f;


	return op;
}