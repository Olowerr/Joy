
RWTexture2DArray<unorm float> lightMaps : register(u0);

#define NumThreadX 16
#define NumThreadY 16
#define SearchRadius 1

[numthreads(NumThreadX, NumThreadY, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	unorm float result = 0.f;
	unorm float foundValue;
	uint foundCount = 0;

	/*for (int x = -SearchRadius; x <= SearchRadius; x++)
	{
		foundValue = lightMaps.Load(int4(DTid.x + x, DTid.y, 0, 0)).r;
		if (foundValue >= 0.2f)
		{
			foundCount++;
			result += foundValue;
		}
	}

	for (int y = -SearchRadius; y <= SearchRadius; y++)
	{
		foundValue = lightMaps.Load(int4(DTid.x, DTid.y + y, 0, 0)).r;
		if (foundValue >= 0.2f)
		{
			foundCount++;
			result += foundValue;
		}
	}*/

	if (lightMaps.Load(int4(DTid.xy, 0, 0)).r < 0.1)
	{
		for (int x = -SearchRadius; x <= SearchRadius; x++)
		{
			for (int y = -SearchRadius; y <= SearchRadius; y++)
			{
				foundValue = lightMaps.Load(int4(DTid.x + x, DTid.y + y, 0, 0)).r;
				if (foundValue > 0.1)
				{
					foundCount++;
					result += foundValue;
				}
			}
		}
	}
	


	AllMemoryBarrierWithGroupSync();

	if (foundCount)
	{
		lightMaps[DTid] = result / foundCount;
	}
}