#include "JoyPostProcess.h"

JoyPostProcess::JoyPostProcess()
	:blurXCS(nullptr), blurYCS(nullptr), blurUAV(nullptr), blurSRV(nullptr)
{
	bool succeeded = false;
	HRESULT hr;

	std::string shaderData;
	succeeded = Backend::LoadShader(Backend::ShaderPath + "BlurXCS.cso", &shaderData);
	assert(succeeded);

	hr = Backend::GetDevice()->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &blurXCS);
	assert(SUCCEEDED(hr));
	
	succeeded = Backend::LoadShader(Backend::ShaderPath + "BlurYCS.cso", &shaderData);
	assert(succeeded);

	hr = Backend::GetDevice()->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &blurYCS);
	assert(SUCCEEDED(hr));

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

	D3D11_TEXTURE2D_DESC texDesc;
	(*Backend::GetBackBuffer())->GetDesc(&texDesc);
	texDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Format = DXGI_FORMAT_R8_UNORM;

	ID3D11Texture2D* resource;
	hr = Backend::GetDevice()->CreateTexture2D(&texDesc, nullptr, &resource);
	if (FAILED(hr))
	{
		assert(SUCCEEDED(hr));
		return;
	}

	Backend::GetDevice()->CreateUnorderedAccessView(resource, nullptr, &blurUAV);
	Backend::GetDevice()->CreateShaderResourceView(resource, nullptr, &blurSRV);
	resource->Release();
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
	//Backend::GetDeviceContext()->CopyResource(*Backend::GetBackBuffer(), Backend::system->renderTexture);
	//return;

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

}
