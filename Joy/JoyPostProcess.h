#pragma once
#include "Backend.h"

class JoyPostProcess
{
public:
	JoyPostProcess();
	void Shutdown();

	void ApplyGlow();

	const UINT NumThreadX = 16;
	const UINT NumThreadY = 9;

private:
	ID3D11ComputeShader* blurXCS;
	ID3D11ComputeShader* blurYCS;

	ID3D11UnorderedAccessView* blurUAV;
	ID3D11ShaderResourceView* blurSRV;

};