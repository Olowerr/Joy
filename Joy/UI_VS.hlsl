struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXTURECOORD;
};

cbuffer transform : register(b0)
{
	float2 pos;
	float2 size;
};

cbuffer matrices : register(b1)
{
	float4x4 projection;
}

VS_OUT main( float2 inPos : POSITION )
{
	VS_OUT output;
	output.uv = inPos * float2(1.f, -1.f);
	output.pos = mul(float4(pos + inPos * size, 0.1f, 1.f), projection);

	return output;
}