RWTexture2D<unorm float> target : register(u0);
Texture2D<unorm float> source : register(t0);

#define Range 10
#define Factor 1.f

cbuffer dirData : register(b0)
{
	int dir;
	int3 pad;
}

[numthreads(16, 9, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	float glow = 0.f;

	if (dir == 0)
	{
		for (int x = -Range; x <= Range; x++)
			glow += source.Load(int3(DTid.x + x, DTid.y, 0)) * ((Range - uint(abs(x))) / (float)Range);
	}
	else
	{
		for (int y = -Range; y <= Range; y++)
			glow += source.Load(int3(DTid.x, DTid.y + y, 0)) * ((Range - uint(abs(y))) / (float)Range);
	}
	
	AllMemoryBarrier();
	target[DTid.xy] = (glow / float(Range * 2 + 1)) * Factor;
}