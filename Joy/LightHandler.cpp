#include "LightHandler.h"

HLight::HLight(ObjectRender& objRender)
	:objRender(objRender)
{

	bool succeeded = false;

	succeeded = InitiateBuffers();
	assert(succeeded);

	succeeded = InitiateShaders();
	assert(succeeded);

	succeeded = InitiateRasterizerStates();
	assert(succeeded);

	succeeded = InitiateShadowMap();
	assert(succeeded);

	lightViewPort.TopLeftX = 0.f;
	lightViewPort.TopLeftY = 0.f;
	lightViewPort.MaxDepth = 1.f;
	lightViewPort.MinDepth = 0.f;
}

void HLight::Shutdown()
{
	lightVS->Release();
	noCullingRS->Release();
	lightPS->Release();
	lightDataBuffer->Release();

	shadowMapDSV->Release();
	shadowMapSRV->Release();
	lightViewProjectBuffer->Release();
	frontFaceCullingRS->Release();
}

HLight::~HLight()
{
}

void HLight::GenerateLightMaps(Object** objects, UINT amount)
{
	ID3D11Device* device = Backend::GetDevice();
	ID3D11DeviceContext* deviceContext = Backend::GetDeviceContext();
	HRESULT hr;

	deviceContext->IASetInputLayout(objRender.GetObjectInputLayout());
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	DrawShadowMap(objects, amount);

	lightViewPort.Width = (float)LightMapWidth;
	lightViewPort.Height = (float)LightMapHeight;
	deviceContext->RSSetViewports(1, &lightViewPort);

	deviceContext->VSSetShader(lightVS, nullptr, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &lightViewProjectBuffer);

	deviceContext->RSSetState(noCullingRS);

	deviceContext->PSSetShader(lightPS, nullptr, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightDataBuffer);
	deviceContext->PSSetShaderResources(0, 1, &shadowMapSRV);

	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Width = LightMapWidth;
	texDesc.Height = LightMapHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	

	ID3D11Texture2D* resource{};
	ID3D11RenderTargetView* tempRTV{};

	for (UINT i = 0; i < amount; i++)
	{
		hr = device->CreateTexture2D(&texDesc, nullptr, &resource);
		if (FAILED(hr))
			continue;

		hr = device->CreateShaderResourceView(resource, nullptr, objects[i]->GetLightMapSRV());
		if (FAILED(hr))
		{
			resource->Release();
			continue;
		}

		hr = device->CreateRenderTargetView(resource, nullptr, &tempRTV);
		if (FAILED(hr))
		{
			resource->Release();
			(*objects[i]->GetLightMapSRV())->Release();
			continue;
		}

		deviceContext->OMSetRenderTargets(1, &tempRTV, nullptr);
		objects[i]->DrawGeometry();

		resource->Release();
		tempRTV->Release();
	}

	deviceContext->RSSetState(nullptr);
}

void HLight::DrawShadowMap(Object** objects, UINT amount)
{
	ID3D11DeviceContext* deviceContext = Backend::GetDeviceContext();
	ID3D11RenderTargetView* nullRTV{};

	lightViewPort.Width = (float)ShadowMapWidth;
	lightViewPort.Height = (float)ShadowMapHeight;
	deviceContext->RSSetViewports(1, &lightViewPort);

	deviceContext->VSSetShader(objRender.GetObjectVS(), nullptr, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &lightViewProjectBuffer);

	deviceContext->RSSetState(frontFaceCullingRS);

	deviceContext->PSSetShader(nullptr, nullptr, 0);
	deviceContext->OMSetRenderTargets(0, &nullRTV, shadowMapDSV);

	for (UINT i = 0; i < amount; i++)
		objects[i]->DrawGeometry();
	
	deviceContext->OMSetRenderTargets(0, &nullRTV, nullptr);
}

bool HLight::InitiateShadowMap()
{
	ID3D11Device* device = Backend::GetDevice();
	HRESULT hr;

	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.ArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.Height = ShadowMapHeight;
	texDesc.Width = ShadowMapWidth;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D* resource;
	hr = device->CreateTexture2D(&texDesc, nullptr, &resource);
	if (FAILED(hr))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	hr = device->CreateShaderResourceView(resource, &srvDesc, &shadowMapSRV);
	if (FAILED(hr))
	{
		resource->Release();
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	dsvDesc.Flags = 0;
	hr = device->CreateDepthStencilView(resource, &dsvDesc, &shadowMapDSV);
	if (FAILED(hr))
	{
		shadowMapSRV->Release();
		resource->Release();
		return false;
	}

	resource->Release();

	Backend::GetDeviceContext()->ClearDepthStencilView(shadowMapDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return true;
}

bool HLight::InitiateShaders()
{
	std::string shaderData;
	bool succeeded = false;
	HRESULT hr;

	succeeded = Backend::LoadShader(Backend::ShaderPath + "LightMapVS.cso", &shaderData);
	if (!succeeded)
		return false;

	hr = Backend::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &lightVS);
	if (FAILED(hr))
		return false;

	succeeded = Backend::LoadShader(Backend::ShaderPath + "LightMapPS.cso", &shaderData);
	if (!succeeded)
		return false;
	
	hr = Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &lightPS);
	if (FAILED(hr))
		return false;

	return true;
}

bool HLight::InitiateBuffers()
{
	bool succeeded = false;
	HRESULT hr;

	using namespace DirectX;
	XMVECTOR SUNPOS = XMVectorSet(10.f, 10.f, -10.f, 0.f);
	XMVECTOR SUNDIR = XMVectorSet(-1.f, -1.f, 1.f, 0.f);

	SUN.strength = 1.f;
	XMStoreFloat3(&SUN.direction, -XMVector3Normalize(SUNDIR));
	XMStoreFloat4x4(&SUN.viewProject, XMMatrixTranspose(
		XMMatrixLookToLH(SUNPOS, SUNDIR, XMVectorSet(0.f, 1.f, 0.f, 0.f)) * XMMatrixOrthographicLH(15.f, 15.f, 0.1f, 30.f)));

	hr = Backend::CreateConstCBuffer(&lightDataBuffer, &SUN, sizeof(DirectionalLight));
	if (FAILED(hr))
		return false;

	hr = Backend::CreateConstCBuffer(&lightViewProjectBuffer, &SUN.viewProject, sizeof(XMFLOAT4X4));
	if (FAILED(hr))
		return false;

	return true;
}

bool HLight::InitiateRasterizerStates()
{
	HRESULT hr;
	
	D3D11_RASTERIZER_DESC rsDesc{};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.DepthBias = 0;
	rsDesc.SlopeScaledDepthBias = 0.f;
	rsDesc.DepthBiasClamp = 0.f;
	rsDesc.DepthClipEnable = true; // why not false in shadowMapping..?
	rsDesc.ScissorEnable = false;
	rsDesc.MultisampleEnable = false;
	rsDesc.AntialiasedLineEnable = false;

	hr = Backend::GetDevice()->CreateRasterizerState(&rsDesc, &frontFaceCullingRS);
	if (FAILED(hr))
		return false;
	
	rsDesc.CullMode = D3D11_CULL_NONE;
	hr = Backend::GetDevice()->CreateRasterizerState(&rsDesc, &noCullingRS);
	if (FAILED(hr))
		return false;

	return true;
}
