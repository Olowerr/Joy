struct Vertex
{
	float4 pos : SV_POSITION;
	float2 uv : UV;
};

Vertex main(Vertex input)
{
	return input;
}