#include "JoyPostProcess.h"
#include <iostream>

JoyPostProcess::JoyPostProcess()
	:blurCS(nullptr), xBlurUAV(nullptr), sampleSRV(nullptr)
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
	texDesc.Width = texDesc.Width / 2;
	texDesc.Height = texDesc.Height / 2;

	XGroups = texDesc.Width / NumThreadX;
	YGroups = texDesc.Height / NumThreadY;

	ID3D11Texture2D* resource;
	hr = Backend::GetDevice()->CreateTexture2D(&texDesc, nullptr, &resource);
	if (FAILED(hr))
	{
		assert(SUCCEEDED(hr));
		return;
	}

	int data[4] = { 0, 0, 0, 0 };
	Backend::CreateDynamicCBuffer(&blurSwitch, data, 16);


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
	Backend::GetDevice()->CreateTexture2D(&texDesc, nullptr, &resource);
	Backend::GetDevice()->CreateUnorderedAccessView(resource, nullptr, &yBlurUAV);
	Backend::GetDevice()->CreateShaderResourceView(resource, nullptr, &yBlurSRV);
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
	blurCS->Release();

	xBlurUAV->Release();
	sampleSRV->Release();
}

void JoyPostProcess::ApplyGlow()
{
	//Backend::GetDeviceContext()->CopyResource(*Backend::GetBackBuffer(), *Backend::GetMainBuffer());
	//return;
	
	DownSample();

	std::chrono::time_point<std::chrono::system_clock> frameStart = std::chrono::system_clock::now();

	static ID3D11RenderTargetView* nullRTV{};
	static ID3D11UnorderedAccessView* nullUAV{};
	static ID3D11ShaderResourceView* nullSRV[2]{};

	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();
	
	// Unbind sampleRTV
	devContext->OMSetRenderTargets(1, &nullRTV, nullptr);

	int dir = 0;
	Backend::UpdateBuffer(blurSwitch, &dir, 4);

	devContext->CSSetShader(blurCS, nullptr, 0);
	devContext->CSSetConstantBuffers(0, 1, &blurSwitch);

	// Horisontal Blur
	devContext->CSSetUnorderedAccessViews(0, 1, &xBlurUAV, nullptr);
	devContext->CSSetShaderResources(0, 1, &sampleSRV);
	devContext->Dispatch(XGroups, YGroups, 1);


	// Vertical Blur
	dir = 1;
	Backend::UpdateBuffer(blurSwitch, &dir, 4);
	devContext->CSSetUnorderedAccessViews(0, 1, &yBlurUAV, nullptr);
	devContext->CSSetShaderResources(0, 1, &xBlurSRV);
	devContext->Dispatch(XGroups, YGroups, 1);

	// Unbind
	devContext->CSSetShaderResources(0, 2, nullSRV);
	devContext->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
	devContext->PSSetShaderResources(6, 1, nullSRV);

	UpSample();

	std::chrono::duration<float> time = std::chrono::system_clock::now() - frameStart;
	std::cout << time.count() << "\n";

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

	devContext->VSSetShader(sampleVS, nullptr, 0);

	devContext->PSSetShader(samplePS, nullptr, 0);
	devContext->PSSetShaderResources(5, 1, Backend::GetMainSRV());

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

	devContext->VSSetShader(sampleVS, nullptr, 0);

	devContext->PSSetShader(upSamplePS, nullptr, 0);
	devContext->PSSetShaderResources(5, 1, Backend::GetMainSRV());
	devContext->PSSetShaderResources(6, 1, &yBlurSRV);

	devContext->RSSetViewports(1, &Backend::GetDefaultViewport());

	devContext->OMSetRenderTargets(1, Backend::GetBackBufferRTV(), nullptr);

	devContext->Draw(4, 0);

	devContext->PSSetShaderResources(5, 1, &nullSRV);
	devContext->PSSetShaderResources(6, 1, &nullSRV);
}

bool JoyPostProcess::LoadShaders()
{
	std::string shaderData;
	
	if (!Backend::LoadShader(Backend::ShaderPath + "BlurCS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &blurCS)))
		return false;
		

	if (!Backend::LoadShader(Backend::ShaderPath + "BlurVS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &sampleVS)))
		return false;


	if (!Backend::LoadShader(Backend::ShaderPath + "BlurPS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &samplePS)))
		return false;

	if (!Backend::LoadShader(Backend::ShaderPath + "UpSamplePS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &upSamplePS)))
		return false;

	return true;
}
