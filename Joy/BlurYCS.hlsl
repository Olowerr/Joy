RWTexture2D<unorm float> target : register(u0);
Texture2D<unorm float> blurSource : register(t1);

#define Range 10
#define Factor 1.f

[numthreads(16, 9, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	return;

	float glow = 0.f;

	for (int y = -Range; y <= Range; y++)
		glow += blurSource.Load(int3(DTid.x, DTid.y + y, 0)) * ((Range - uint(abs(y))) / (float)Range) * Factor;

	AllMemoryBarrier();
	target[DTid.xy] = (glow / float(Range * 2 + 1)) * Factor;
}