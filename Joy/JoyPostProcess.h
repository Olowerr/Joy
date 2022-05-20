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

	static void CalcGlowAmount(float joyFuel);
	static void SetActive(bool active);

private:
	static JoyPostProcess* me;
	bool active;

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
	ID3D11Buffer* glowColourBuffer;
	const float NoFuelColour[4] = { 1.f, 0.f, 0.f, 0.f };
	const float FuelColour2[4] = { 1.f, 0.4f, 0.0f, 0.f };
	const float FuelColour1[4] = { 1.f, 0.f, 1.0f, 0.f };
	const float MaxFuelColour[4] = { 0.43137f, 0.96471f, 1.f, 0.f };
	float glowColour[4];

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

/*
	if (Factor > 0.6f)
	{
		me->glowColour[0] = (1.f - Factor) * me->HalfFuelColour[0] + Factor * me->MaxFuelColour[0];
		me->glowColour[1] = (1.f - Factor) * me->HalfFuelColour[1] + Factor * me->MaxFuelColour[1];
		me->glowColour[2] = (1.f - Factor) * me->HalfFuelColour[2] + Factor * me->MaxFuelColour[2];

	}
	else if (Factor < 0.4f)
	{
		me->glowColour[0] = (1.f - Factor) * me->NoFuelColour[0] + Factor * me->HalfFuelColour[0];
		me->glowColour[1] = (1.f - Factor) * me->NoFuelColour[1] + Factor * me->HalfFuelColour[1];
		me->glowColour[2] = (1.f - Factor) * me->NoFuelColour[2] + Factor * me->HalfFuelColour[2];

	}
	else
	{
		me->glowColour[0] = 1.f;
		me->glowColour[1] = 0.4f;
		me->glowColour[2] = 0.f;
	}

*/