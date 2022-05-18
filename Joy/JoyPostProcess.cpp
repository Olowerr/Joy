#include "JoyPostProcess.h"
#include <iostream>

JoyPostProcess::JoyPostProcess()
	:blurXCS(nullptr), blurYCS(nullptr), blurUAV(nullptr), blurSRV(nullptr)
{
	bool succeeded = false;
	HRESULT hr;

	/*D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Format = DXGI_FORMAT_R8_UNORM;
	texDesc.ArraySize = 1;
	texDesc.Width = Backend::GetWindowWidth();
	texDesc.Height = Backend::GetWindowHeight();
	texDesc.CPUAccessFlags = 0;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;*/

	succeeded = LoadShaders();
	assert(succeeded);


	struct Data
	{
		DirectX::XMFLOAT4 pos[4] = {
		{-1.f, 1.f, 0.f, 1.f},
		{1.f, 1.f, 0.f, 1.f},
		{-1.f, -1.f, 0.f, 1.f},
		{1.f, -1.f, 0.f, 1.f}
	};
		DirectX::XMFLOAT2 uv[4] = {
		{0.f, 0.f},
		{1.f, 0.f},
		{0.f, 1.f},
		{1.f, 1.f}
	};
	};
	Data vData;

	hr = Backend::CreateVertexBuffer(&quadBuffer, &vData, sizeof(Data));
	assert(SUCCEEDED(hr));

	D3D11_TEXTURE2D_DESC texDesc;
	(*Backend::GetBackBuffer())->GetDesc(&texDesc);
	texDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.Format = DXGI_FORMAT_R8_UNORM;
	texDesc.Width = texDesc.Width / 2;
	texDesc.Height = texDesc.Height / 2;

	ID3D11Texture2D* resource;
	hr = Backend::GetDevice()->CreateTexture2D(&texDesc, nullptr, &resource);
	if (FAILED(hr))
	{
		assert(SUCCEEDED(hr));
		return;
	}

	Backend::GetDevice()->CreateUnorderedAccessView(resource, nullptr, &blurUAV);
	Backend::GetDevice()->CreateShaderResourceView(resource, nullptr, &blurSRV);
	Backend::GetDevice()->CreateRenderTargetView(resource, nullptr, &sampleRTV);
	resource->Release();



	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)texDesc.Width;
	viewport.Height = (float)texDesc.Height;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
}

void JoyPostProcess::Shutdown()
{
	blurXCS->Release();
	blurYCS->Release();

	blurUAV->Release();
	blurSRV->Release();
}

void JoyPostProcess::ApplyGlow()
{
	ApplyGlow2();

	Backend::GetDeviceContext()->CopyResource(*Backend::GetBackBuffer(), *Backend::GetMainBuffer());
	return;

	std::chrono::time_point<std::chrono::system_clock> frameStart = std::chrono::system_clock::now();

	static ID3D11RenderTargetView* nullRTV{};
	static ID3D11UnorderedAccessView* nullUAV{};
	static ID3D11ShaderResourceView* nullSRV[2]{};

	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();
	
	// Unbind MainRTV
	devContext->OMSetRenderTargets(1, &nullRTV, nullptr);

	// Horisontal Blur
	devContext->CSSetShader(blurXCS, nullptr, 0);
	devContext->CSSetUnorderedAccessViews(0, 1, &blurUAV, nullptr);
	devContext->CSSetShaderResources(0, 1, Backend::GetMainSRV());
	devContext->Dispatch(Backend::GetWindowWidth() / NumThreadX, Backend::GetWindowHeight() / NumThreadY, 1);

	// Vertical Blur
	devContext->CSSetShader(blurYCS, nullptr, 0);
	devContext->CSSetUnorderedAccessViews(0, 1, Backend::GetBackBufferUAV(), nullptr);
	devContext->CSSetShaderResources(1, 1, &blurSRV);
	devContext->Dispatch(Backend::GetWindowWidth() / NumThreadX, Backend::GetWindowHeight() / NumThreadY, 1);

	// Unbind
	devContext->CSSetShaderResources(0, 2, nullSRV);
	devContext->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);

	std::chrono::duration<float> time = std::chrono::system_clock::now() - frameStart;
	std::cout << time.count() << "\n";
}

void JoyPostProcess::ApplyGlow2()
{
	static const UINT Stride = sizeof(DirectX::XMFLOAT4) + sizeof(DirectX::XMFLOAT2), Offset = 0;

	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

	devContext->IASetInputLayout(il);
	devContext->IASetVertexBuffers(0, 1, &quadBuffer, &Stride, &Offset);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	devContext->VSSetShader(sampleVS, nullptr, 0);

	devContext->PSSetShader(samplePS, nullptr, 0);
	devContext->PSSetShaderResources(5, 1, Backend::GetMainSRV());

	devContext->OMSetRenderTargets(1, &sampleRTV, nullptr);

	devContext->Draw(4, 0);
}

bool JoyPostProcess::LoadShaders()
{
	std::string shaderData;
	
	if (!Backend::LoadShader(Backend::ShaderPath + "BlurXCS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &blurXCS)))
		return false;
	

	if (!Backend::LoadShader(Backend::ShaderPath + "BlurVS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &sampleVS)))
		return false;

	D3D11_INPUT_ELEMENT_DESC desc[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0,16, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	if (FAILED(Backend::GetDevice()->CreateInputLayout(desc, 2, shaderData.c_str(), shaderData.length(), &il)))
		return false;

	if (!Backend::LoadShader(Backend::ShaderPath + "BlurPS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &samplePS)))
		return false;


	return true;
}
