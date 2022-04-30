struct GSData
{
	float4 pos : SV_POSITION;
	float3 wsPos : WS_POSITION;
	float3 normal : NORMAL;
};

[maxvertexcount(3)]
void main(triangle GSData input[3], inout TriangleStream< GSData > output)
{
	float2 triangleCenter = (input[0].pos.xy + input[1].pos.xy + input[2].pos.xy) / 3.f;

	for (uint i = 0; i < 3; i++)
	{
		GSData element;
		element.pos = float4(((input[i].pos.xy - triangleCenter) + 0.0f / 512.f) + triangleCenter, 0.f, 1.f);
		element.wsPos = input[i].wsPos;
		element.normal = input[i].normal;
		output.Append(element);
	}
}