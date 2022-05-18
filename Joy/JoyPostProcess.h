#pragma once
#include "Backend.h"
#include <DirectXMath.h>

class JoyPostProcess
{
public:
	JoyPostProcess();
	void Shutdown();

	void ApplyGlow();
	void ApplyGlow2();

	const UINT NumThreadX = 16;
	const UINT NumThreadY = 9;

private:
	ID3D11ComputeShader* blurXCS;
	ID3D11ComputeShader* blurYCS;

	ID3D11UnorderedAccessView* blurUAV;
	ID3D11ShaderResourceView* blurSRV;

	// Down/Up sampling
	ID3D11VertexShader* sampleVS;
	ID3D11PixelShader* samplePS;
	D3D11_VIEWPORT viewport;
	ID3D11RenderTargetView* sampleRTV;

	ID3D11Buffer* quadBuffer;
	const UINT VertexCount = 4;


	bool LoadShaders();

	// temp
	ID3D11InputLayout* il;
};