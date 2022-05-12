RWTexture2D<unorm float4> bbUAV : register(u0);
Texture2D<unorm float4> bbCopy : register(t0);

#define Range 10

[numthreads(16, 9, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	const float4 colour = bbCopy.Load(DTid);
	float result = 0.f;

	for (int x = -Range; x <= Range; x++)
		result += bbCopy.Load(int3(DTid.x + x, DTid.y, 0)).a;

	//for (int y = -Range; y <= Range; y++)
	//	result += bbCopy.Load(int3(DTid.x, DTid.y + y, 0)).a;
	//
	AllMemoryBarrier();

	result /= (Range * 2 + 1);
	result *= 0.2f;
	bbUAV[DTid.xy] = colour + float4(result, result, result, result);// float4(colour.rbg, result);
	
	//AllMemoryBarrier();

	//result = 0.f;

	//AllMemoryBarrier();

	//result /= (Range * 2 + 1);
	//result *= 0.2f;
	//bbUAV[DTid.xy] = bbCopy.Load(DTid) + float4(result, result, result, 0.f);

}