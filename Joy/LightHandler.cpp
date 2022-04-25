#include "LightHandler.h"

HLight::HLight()
{
	ID3D11Device* device = Backend::GetDevice();
	
	std::string shaderData;
	bool succeeded = false;
	HRESULT hr;

	using namespace DirectX;
	XMVECTOR SUNPOS = XMVectorSet(10.f, 10.f, -20.f, 0.f);
	XMVECTOR SUNDIR = XMVectorSet(-1.f, -1.f, 1.f, 0.f);

	SUN.strength = 1.f;
	XMStoreFloat3(&SUN.direction, -XMVector3Normalize(SUNDIR));
	XMStoreFloat4x4(&SUN.viewProject, 
		XMMatrixLookToLH(SUNPOS, SUNDIR, XMVectorSet(0.f, 1.f, 0.f, 0.f)) * XMMatrixOrthographicLH(15.f, 15.f, 0.1f, 30.f));

	hr = Backend::CreateConstCBuffer(&lightPSBuffer, &SUN, sizeof(DirectionalLight));
	assert(FAILED(hr));

	XMFLOAT4X4 lightViewProject;
	XMStoreFloat4x4(&lightViewProject, XMMatrixTranspose(XMLoadFloat4x4(&SUN.viewProject)));
	hr = Backend::CreateConstCBuffer(&lightVSBuffer, &lightViewProject, sizeof(XMFLOAT4X4));
	assert(FAILED(hr));
	
	succeeded = InitiateShadowMap();
	assert(succeeded);

	succeeded = Backend::LoadShader(Backend::ShaderPath + "LightMapVS.cso", &shaderData);
	assert(succeeded);
	hr = device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &lightVS);
	assert(FAILED(hr));

	succeeded = Backend::LoadShader(Backend::ShaderPath + "LightMapPS.cso", &shaderData);
	assert(succeeded);
	hr = device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &lightPS);
	assert(FAILED(hr));


}

void HLight::Shutdown()
{
}

HLight::~HLight()
{
}

void HLight::CreateLightMaps(Object** objects, UINT amount)
{
	ID3D11DeviceContext* deviceContext = Backend::GetDeviceContext();
	
	DrawShadowMaps(objects, amount);

	deviceContext->VSSetShader(lightVS, nullptr, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &lightVSBuffer);
	
	deviceContext->PSSetShader(lightPS, nullptr, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightPSBuffer);

	//deviceContext->OMSetRenderTargets();

	Mesh* tempMesh = nullptr;

	for (UINT i = 0; i < amount; i++)
	{
		tempMesh = objects[i]->GetMesh();

		deviceContext->IASetVertexBuffers(0, 1, &tempMesh->vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
		//deviceContext->IASetIndexBuffer(tempMesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		//deviceContext->IASetInputLayout();
	}

}

void HLight::DrawShadowMap(Object** objects, UINT amount)
{
	ID3D11DeviceContext* deviceContext = Backend::GetDeviceContext();
	ID3D11RenderTargetView* nullRTV = nullptr;

	//deviceContext->IASetInputLayout();
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//deviceContext->VSSetShader(standardVS, nullptr, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &lightVSBuffer);

	deviceContext->PSSetShader(nullptr, nullptr, 0);
	deviceContext->OMSetRenderTargets(0, &nullRTV, shadowMapDSV);

	Mesh* tempMesh = nullptr;
	for (UINT i = 0; i < amount; i++)
	{
		tempMesh = objects[i]->GetMesh();

		deviceContext->IASetVertexBuffers(0, 1, &tempMesh->vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
		//deviceContext->IASetIndexBuffer(tempMesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	}

}

bool HLight::InitiateShadowMap()
{
	ID3D11Device* device = Backend::GetDevice();
	HRESULT hr;

	D3D11_TEXTURE2D_DESC texDesc;
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

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
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

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(resource, &dsvDesc, &shadowMapDSV);
	if (FAILED(hr))
	{
		resource->Release();
		return false;
	}

	resource->Release();

	return true;
}
