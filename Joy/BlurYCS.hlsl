RWTexture2D<unorm float> target : register(u0);
Texture2D<unorm float> blurSource : register(t1);

#define Range 5
#define Factor 2.f

[numthreads(16, 9, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	float glow = 0.f;

	for (int y = -Range; y <= Range; y++)
		glow += blurSource.Load(int3(DTid.x, DTid.y + y, 0)) * ((Range - uint(abs(y))) / (float)Range) * Factor;

	//glow /= float(Range * 2 + 1);

	AllMemoryBarrier();
	//target[DTid.xy] = float4(mainSource.Load(DTid).rgb + float3(glow, glow, glow) * (mainSource.Load(DTid).a < 0.5f), 0.f);
	target[DTid.xy] = glow / (Range * 2 + 1);

	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
	// remove mainBuffer
}