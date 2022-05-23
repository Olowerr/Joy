#include "JoyPostProcess.h"
#include <iostream>

JoyPostProcess* JoyPostProcess::me = nullptr;

JoyPostProcess::JoyPostProcess()
	:blurCS(nullptr), xBlurUAV(nullptr), sampleSRV(nullptr)
	, NumThreadX(16), NumThreadY(9)
	, SampleTexX(NumThreadX * 30), SampleTexY(NumThreadY * 30)
	, XGroups(SampleTexX / NumThreadX), YGroups(SampleTexY / NumThreadY)
	, glowColour{ 0.f, 0.f, 1.f, 0.f }
	, active(true)
{
	me = this;

	bool succeeded = false;
	HRESULT hr;

	succeeded = LoadShaders();
	assert(succeeded);

	DirectX::XMFLOAT4 pos[4] = {
		{-1.f, 1.f, 0.f, 1.f},
		{1.f, 1.f, 0.f, 1.f},
		{-1.f, -1.f, 0.f, 1.f},
		{1.f, -1.f, 0.f, 1.f}
	};
	hr = Backend::CreateVertexBuffer(&quadBuffer, pos, sizeof(pos));
	assert(SUCCEEDED(hr));


	D3D11_TEXTURE2D_DESC texDesc;
	(*Backend::GetBackBuffer())->GetDesc(&texDesc);
	texDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.Format = DXGI_FORMAT_R8_UNORM;
	texDesc.Width = SampleTexX;
	texDesc.Height = SampleTexY;

	ID3D11Texture2D* resource;
	hr = Backend::GetDevice()->CreateTexture2D(&texDesc, nullptr, &resource);
	if (FAILED(hr))
	{
		assert(SUCCEEDED(hr));
		return;
	}

	Backend::GetDevice()->CreateShaderResourceView(resource, nullptr, &sampleSRV);
	Backend::GetDevice()->CreateRenderTargetView(resource, nullptr, &sampleRTV);
	resource->Release();

	hr = Backend::GetDevice()->CreateTexture2D(&texDesc, nullptr, &resource);
	if (FAILED(hr))
	{
		assert(SUCCEEDED(hr));
		return;
	}
	Backend::GetDevice()->CreateUnorderedAccessView(resource, nullptr, &xBlurUAV);
	Backend::GetDevice()->CreateShaderResourceView(resource, nullptr, &xBlurSRV);
	resource->Release();

	hr = Backend::GetDevice()->CreateTexture2D(&texDesc, nullptr, &resource);
	if (FAILED(hr))
	{
		assert(SUCCEEDED(hr));
		return;
	}
	Backend::GetDevice()->CreateUnorderedAccessView(resource, nullptr, &yBlurUAV);
	Backend::GetDevice()->CreateShaderResourceView(resource, nullptr, &yBlurSRV);
	resource->Release();

	Backend::CreateDynamicCBuffer(&blurSwitch, nullptr, 16);
	Backend::CreateDynamicCBuffer(&glowColourBuffer, nullptr, 32);

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)texDesc.Width;
	viewport.Height = (float)texDesc.Height;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

void JoyPostProcess::Shutdown()
{
	blurCS->Release();

	sampleRTV->Release();
	sampleSRV->Release();

	xBlurUAV->Release();
	xBlurSRV->Release();

	yBlurSRV->Release();
	yBlurUAV->Release();

	blurSwitch->Release();
	glowColourBuffer->Release();

	screenQuadVS->Release();
	downSamplePS->Release();

	upSamplePS->Release();

	quadBuffer->Release();
}

void JoyPostProcess::ApplyGlow()
{
	if (!active)
	{
		Backend::GetDeviceContext()->CopyResource(*Backend::GetBackBuffer(), *Backend::GetMainBuffer());
		return;
	}

	auto frameStart = std::chrono::system_clock::now();

	DownSample();

	static ID3D11RenderTargetView* nullRTV[2]{};
	static ID3D11UnorderedAccessView* nullUAV{};
	static ID3D11ShaderResourceView* nullSRV[2]{};
	static float data[2] = { 0.f, 2.f };

	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

	// Unbind sampleRTV
	devContext->OMSetRenderTargets(2, nullRTV, nullptr);

#ifdef _DEBUG
	if (ImGui::Begin("Glow"))
	{
		ImGui::InputFloat("Glow Strength", &data[1], 0.1f);
		data[1] = data[1] < 0.f ? 0.f : data[1];

		//ImGui::ColorEdit3("Glow Colour", glowColour);
	}
	ImGui::End();
#endif // _DEBUG

	data[0] = 0.f;
	Backend::UpdateBuffer(blurSwitch, data, 8);
	Backend::UpdateBuffer(glowColourBuffer, &glowColour, 32);

	devContext->CSSetShader(blurCS, nullptr, 0);
	devContext->CSSetConstantBuffers(0, 1, &blurSwitch);

	// Horisontal Blur
	devContext->CSSetUnorderedAccessViews(0, 1, &xBlurUAV, nullptr);
	devContext->CSSetShaderResources(0, 1, &sampleSRV);
	devContext->Dispatch(XGroups, YGroups, 1);


	// Vertical Blur
	data[0] = 1.f;

	Backend::UpdateBuffer(blurSwitch, data, 8);
	devContext->CSSetUnorderedAccessViews(0, 1, &yBlurUAV, nullptr);
	devContext->CSSetShaderResources(0, 1, &xBlurSRV);
	devContext->Dispatch(XGroups, YGroups, 1);

	// Unbind
	devContext->CSSetShaderResources(0, 2, nullSRV);
	devContext->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
	devContext->PSSetShaderResources(6, 1, nullSRV);

	UpSample();

	std::chrono::duration<float> time = std::chrono::system_clock::now() - frameStart;
	//std::cout << time.count() << "\n";

}

void JoyPostProcess::CalcGlowAmount(float fuel)
{
	const float MaxFuel = 10.f;
	//static float fuel = MaxFuel;

#ifdef  _DEBUG
	if (ImGui::Begin("asd"))
	{
		ImGui::InputFloat("smolpp", &fuel, 0.1f);
	}
	ImGui::End();

#endif //  _DEBUG

	fuel = fuel < 0.f ? 0.f : fuel > MaxFuel ? MaxFuel : fuel;

	float Factor = fuel / MaxFuel;

	if (Factor >= 0.666f)
	{
		float factor2 = (Factor - 0.666f) / 0.333f;
		me->glowColour[0] = (1.f - factor2) * me->FuelColour1[0] + factor2 * me->MaxFuelColour[0];
		me->glowColour[1] = (1.f - factor2) * me->FuelColour1[1] + factor2 * me->MaxFuelColour[1];
		me->glowColour[2] = (1.f - factor2) * me->FuelColour1[2] + factor2 * me->MaxFuelColour[2];
	}

	else if (Factor >= 0.333f)
	{
		float factor2 = (Factor - 0.333f) / 0.333f;
		me->glowColour[0] = (1.f - factor2) * me->FuelColour2[0] + factor2 * me->FuelColour1[0];
		me->glowColour[1] = (1.f - factor2) * me->FuelColour2[1] + factor2 * me->FuelColour1[1];
		me->glowColour[2] = (1.f - factor2) * me->FuelColour2[2] + factor2 * me->FuelColour1[2];
	}

	else
	{
		float factor2 = Factor / 0.333f;
		me->glowColour[0] = (1.f - factor2) * me->NoFuelColour[0] + factor2 * me->FuelColour2[0];
		me->glowColour[1] = (1.f - factor2) * me->NoFuelColour[1] + factor2 * me->FuelColour2[1];
		me->glowColour[2] = (1.f - factor2) * me->NoFuelColour[2] + factor2 * me->FuelColour2[2];

	}

	//printf("%.3f, %.3f, %.3f\n", me->glowColour[0], me->glowColour[1], me->glowColour[2]);

}

void JoyPostProcess::SetActive(bool active)
{
	me->active = active;
}

void JoyPostProcess::DownSample()
{
	static const UINT Stride = sizeof(DirectX::XMFLOAT4), Offset = 0;
	static ID3D11RenderTargetView* nullRTV{};
	static ID3D11ShaderResourceView* nullSRV{};

	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

	devContext->OMSetRenderTargets(1, &nullRTV, nullptr);

	devContext->IASetInputLayout(Backend::GetShaderStorage().posOnlyInputLayout);
	devContext->IASetVertexBuffers(0, 1, &quadBuffer, &Stride, &Offset);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	devContext->VSSetShader(screenQuadVS, nullptr, 0);

	devContext->PSSetShader(downSamplePS, nullptr, 0);
	devContext->PSSetShaderResources(5, 1, Backend::GetBlurSRV());

	devContext->RSSetViewports(1, &viewport);

	devContext->OMSetRenderTargets(1, &sampleRTV, nullptr);

	devContext->Draw(4, 0);

	devContext->PSSetShaderResources(5, 1, &nullSRV);
}

void JoyPostProcess::UpSample()
{
	static const UINT Stride = sizeof(DirectX::XMFLOAT4), Offset = 0;
	static ID3D11RenderTargetView* nullRTV{};
	static ID3D11ShaderResourceView* nullSRV{};

	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

	devContext->OMSetRenderTargets(1, &nullRTV, nullptr);

	devContext->IASetVertexBuffers(0, 1, &quadBuffer, &Stride, &Offset);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	devContext->VSSetShader(screenQuadVS, nullptr, 0);

	devContext->PSSetShader(upSamplePS, nullptr, 0);
	devContext->PSSetShaderResources(5, 1, Backend::GetMainSRV());
	devContext->PSSetShaderResources(6, 1, &yBlurSRV);
	devContext->PSSetShaderResources(7, 1, Backend::GetBlurSRV());
	devContext->PSSetConstantBuffers(5, 1, &glowColourBuffer);

	devContext->RSSetViewports(1, &Backend::GetDefaultViewport());

	devContext->OMSetRenderTargets(1, Backend::GetBackBufferRTV(), nullptr);

	devContext->Draw(4, 0);

	devContext->PSSetShaderResources(5, 1, &nullSRV);
	devContext->PSSetShaderResources(6, 1, &nullSRV);
	devContext->PSSetShaderResources(7, 1, &nullSRV);
}

bool JoyPostProcess::LoadShaders()
{
	std::string shaderData;

	if (!Backend::LoadShader(Backend::ShaderPath + "BlurCS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &blurCS)))
		return false;


	if (!Backend::LoadShader(Backend::ShaderPath + "screenQuadVS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &screenQuadVS)))
		return false;


	if (!Backend::LoadShader(Backend::ShaderPath + "DownSamplePS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &downSamplePS)))
		return false;

	if (!Backend::LoadShader(Backend::ShaderPath + "UpSamplePS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &upSamplePS)))
		return false;

	return true;
}
