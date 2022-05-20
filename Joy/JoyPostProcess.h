#pragma once
#include "Backend.h"
#include <DirectXMath.h>

#include "imgui\imgui.h"
#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui_impl_dx11.h"
class JoyPostProcess
{
public:
	JoyPostProcess();
	void Shutdown();

	void ApplyGlow();


private:
	ID3D11ComputeShader* blurCS;

	// could optimize
	// could optimize
	// could optimize
	// could optimize
	
	// tex 0
	ID3D11RenderTargetView* sampleRTV; // Render to Smol tex
	ID3D11ShaderResourceView* sampleSRV; // Read from SMolTex
	
	// tex 1
	ID3D11UnorderedAccessView* xBlurUAV; // XBlur Target
	ID3D11ShaderResourceView* xBlurSRV; // Read XBLur
	
	// tex 2
	ID3D11UnorderedAccessView* yBlurUAV; // YBLUr Target
	ID3D11ShaderResourceView* yBlurSRV; // Read YBLUR

	ID3D11Buffer* blurSwitch;

	// Down/Up sampling
	ID3D11VertexShader* screenQuadVS;
	ID3D11PixelShader* downSamplePS;
	ID3D11PixelShader* upSamplePS;
	D3D11_VIEWPORT viewport;

	ID3D11Buffer* quadBuffer;
	const UINT VertexCount = 4;

	const UINT NumThreadX;
	const UINT NumThreadY;
	const UINT SampleTexX;
	const UINT SampleTexY;
	const UINT XGroups;
	const UINT YGroups;

	void DownSample();
	void UpSample();

	bool LoadShaders();
};