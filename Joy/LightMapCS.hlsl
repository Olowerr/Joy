
RWTexture2DArray<unorm float> lightMapsUAV : register(u0);
Texture2DArray<unorm float> lightMapsSRV : register(t0);

#define NumThreadX 16
#define NumThreadY 16
#define SearchRadius 1

/*
	Total texels searched = (radius * 2 + 1) ^ 2
	Radius: 1 -> 9  texels
	Radius: 2 -> 25 texels
	Radius: 3 -> 49 texels
	Radius: 4 -> 81 texels
*/

[numthreads(NumThreadX, NumThreadY, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	/*
		Blurs texels using only "legal" texels
		Legal = contains light information
		Minimum light value for legal texels is 0.2
		All illegal texels are 0.0
		0.1 is used for comparing to avoid precision problems

		The shader will also add light data to illegal texels that lay next to a legal one.
		This is necessary because some texels are missed when rasterizing the UVs.
		(Which is due to rasterization rules and partly the resolution of the light map).
		But their information is still used when sampling/loading from the Light map
	*/

	unorm float result = 0.f;
	unorm float foundValue;
	uint foundCount = 0;

	for (int x = -SearchRadius; x <= SearchRadius; x++)
	{
		for (int y = -SearchRadius; y <= SearchRadius; y++)
		{
			foundValue = lightMapsSRV.Load(int4(DTid.x + x, DTid.y + y, DTid.z, 0)).r;

			foundCount += foundValue > 0.1;
			result += foundValue * (foundValue > 0.1);
		}
	}

	AllMemoryBarrierWithGroupSync();

	if (foundCount)
		lightMapsUAV[DTid] = result / foundCount;
	
}