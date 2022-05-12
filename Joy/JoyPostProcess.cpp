#include "JoyPostProcess.h"

JoyPostProcess::JoyPostProcess()
	:bbUAV(Backend::GetBackBufferUAV())
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

	/*
		D3D11_TEXTURE2D_DESC texDesc{};
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

	(*bbUAV)->GetResource( reinterpret_cast<ID3D11Resource**>(& bbCopy));
	D3D11_TEXTURE2D_DESC texDesc;
	bbCopy->GetDesc(&texDesc);
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bbCopy->Release();

	hr = Backend::GetDevice()->CreateTexture2D(&texDesc, nullptr, &bbCopy);
	assert(SUCCEEDED(hr));

	hr = Backend::GetDevice()->CreateShaderResourceView(bbCopy, nullptr, &bbCopySRV);
	assert(SUCCEEDED(hr));
}

void JoyPostProcess::Shutdown()
{
	blurXCS->Release();
	bbCopy->Release();
	bbCopySRV->Release();
}

void JoyPostProcess::ApplyGlow()
{
	return;

	static ID3D11RenderTargetView* nullRTV{};
	static ID3D11UnorderedAccessView* nullUAV{};

	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();
	
	devContext->OMSetRenderTargets(1, &nullRTV, nullptr);

	devContext->CopyResource(bbCopy, *Backend::GetBackBuffer());
	
	devContext->CSSetShader(blurXCS, nullptr, 0);
	devContext->CSSetUnorderedAccessViews(0, 1, bbUAV, nullptr);
	devContext->CSSetShaderResources(0, 1, &bbCopySRV);
	devContext->Dispatch(Backend::GetWindowWidth() / NumThreadX, Backend::GetWindowHeight() / NumThreadY, 1);

	
	devContext->CopyResource(bbCopy, *Backend::GetBackBuffer());
	
	devContext->CSSetShader(blurYCS, nullptr, 0);
	devContext->CSSetUnorderedAccessViews(0, 1, bbUAV, nullptr);
	devContext->CSSetShaderResources(0, 1, &bbCopySRV);
	devContext->Dispatch(Backend::GetWindowWidth() / NumThreadX, Backend::GetWindowHeight() / NumThreadY, 1);


	devContext->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
}
