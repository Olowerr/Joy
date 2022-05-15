cbuffer colourInfo : register(b0)
{
	float4 colour;
}

float4 main() : SV_TARGET
{
	return colour;
}