RWTexture2D<unorm float> target : register(u0);
Texture2D<unorm float4> source : register(t0);

#define Range 5
#define Factor 2.f

[numthreads(16, 9, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	float glow = 0.f;

	for (int x = -Range; x <= Range; x++)
		glow += source.Load(int3(DTid.x + x, DTid.y, 0)).a * ((Range - uint(abs(x))) / (float)Range) * Factor;
	
	AllMemoryBarrier();
	target[DTid.xy] = glow / float(Range * 2 + 1);
}