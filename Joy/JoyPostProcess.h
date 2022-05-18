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

	// tex 0
	ID3D11RenderTargetView* sampleRTV; // Render to Smol tex
	ID3D11ShaderResourceView* sampleSRV; // Read from SMolTex
	
	// tex 1
	ID3D11UnorderedAccessView* xBlurUAV; // XBlur Target
	ID3D11ShaderResourceView* xBlurSRV; // Read XBLur
	
	// tex 2
	ID3D11UnorderedAccessView* yBlurUAV; // YBLUr Target
	ID3D11ShaderResourceView* yBlurSRV; // Read YBLUR


	// Down/Up sampling
	ID3D11VertexShader* sampleVS;
	ID3D11PixelShader* samplePS;
	ID3D11PixelShader* upSamplePS;
	D3D11_VIEWPORT viewport;

	ID3D11Buffer* quadBuffer;
	const UINT VertexCount = 4;
	UINT XGroups;
	UINT YGroups;

	void DownSample();
	void UpSample();

	bool LoadShaders();

	// temp
	ID3D11InputLayout* il;
};