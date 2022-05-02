#include "LightHandler.h"

HLight::HLight()
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



	//temp
	std::string shaderData;
	Backend::LoadShader(Backend::ShaderPath + "ObjVS.cso", &shaderData);

	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	Backend::GetDevice()->CreateInputLayout(inputDesc, 3, shaderData.c_str(), shaderData.length(), &inpLayout);
	Backend::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vs);
}

void HLight::Shutdown()
{
	lightVS->Release();
	noCullingRS->Release();
	lightPS->Release();
	lightDataBuffer->Release();
	lightCS->Release();

	shadowMapDSV->Release();
	shadowMapSRV->Release();
	lightViewProjectBuffer->Release();
	frontFaceCullingRS->Release();
}

HLight::~HLight()
{
}

bool HLight::GenerateLightMaps(Object** objects, UINT amount)
{
	ID3D11Device* device = Backend::GetDevice();
	ID3D11DeviceContext* deviceContext = Backend::GetDeviceContext();
	HRESULT hr;

	deviceContext->IASetInputLayout(inpLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
//	DrawShadowMap(objects, amount);

	lightViewPort.Width = (float)LightMapXY;
	lightViewPort.Height = (float)LightMapXY;
	deviceContext->RSSetViewports(1, &lightViewPort);

	deviceContext->VSSetShader(lightVS, nullptr, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &lightViewProjectBuffer);

	deviceContext->RSSetState(noCullingRS);

	deviceContext->PSSetShader(lightPS, nullptr, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightDataBuffer);
	deviceContext->PSSetShaderResources(0, 1, &shadowMapSRV);

	deviceContext->CSSetShader(lightCS, nullptr, 0);
	
	D3D11_TEXTURE2D_DESC texDesc{};
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

	FillDescriptions(amount, &texDesc, &rtvDesc, &srvDesc, &uavDesc);

	/*  
		Reading a UAV connected to resource with format (R8_UNORM) resulted in the following error:

		D3D11 ERROR: ID3D11DeviceContext::Dispatch:
		The Unordered Access View (UAV) in slot 0 of the Compute Shader unit has the Format (R8_UNORM).
		This format does not support being read from a shader as as UAV. 
		This mismatch is invalid if the shader actually uses the view (e.g. it is not skipped due to shader code branching).
		It was unfortunately not possible to have all hardware implementations support reading this format as a UAV, 
		despite that the format can written to as a UAV. If the shader only needs to perform reads but not writes to this resource, 
		consider using a Shader Resource View instead of a UAV.  [ EXECUTION ERROR #2097381: DEVICE_UNORDEREDACCESSVIEW_FORMAT_LD_UNSUPPORTED]

		Old computer can't read the format (R8_UNORM) in UAVs.
		Solution was to copy the resource to a temporary SRV and read from that.
	*/

	ID3D11Texture2D* tempResource{};
	hr = device->CreateTexture2D(&texDesc, nullptr, &tempResource);
	if (FAILED(hr))
		return false;

	ID3D11Texture2D* resource{};
	hr = device->CreateTexture2D(&texDesc, nullptr, &resource);
	if (FAILED(hr))
		return false;

	ID3D11ShaderResourceView* tempSRV{};
	
	ID3D11RenderTargetView* tempRTV{};
	ID3D11UnorderedAccessView* tempUAV{};

	ID3D11RenderTargetView* nullRTV{};
	ID3D11UnorderedAccessView* nullUAV{};
	ID3D11ShaderResourceView* nullSRV{};

	const UINT NumGroups = LightMapXY / LightMapCSThreadXY;

	for (UINT i = 0; i < amount; i++)
	{
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		srvDesc.Texture2DArray.FirstArraySlice = i;

		hr = device->CreateShaderResourceView(resource, &srvDesc, objects[i]->GetLightMapSRV());
		if (FAILED(hr))
			continue;

		hr = device->CreateRenderTargetView(resource, &rtvDesc, &tempRTV);
		if (FAILED(hr))
		{
			(*objects[i]->GetLightMapSRV())->Release();
			continue;
		}
		
		deviceContext->OMSetRenderTargets(1, &tempRTV, nullptr);
		objects[i]->DrawGeometry();
		
		tempRTV->Release();
	}

	deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
	deviceContext->CopyResource(tempResource, resource);

	srvDesc.Texture2DArray.ArraySize = amount;
	srvDesc.Texture2DArray.FirstArraySlice = 0;

	hr = device->CreateShaderResourceView(tempResource, &srvDesc, &tempSRV);
	if (FAILED(hr))
		return false;

	hr = device->CreateUnorderedAccessView(resource, &uavDesc, &tempUAV);
	if (FAILED(hr))
		return false;

	deviceContext->CSSetUnorderedAccessViews(0, 1, &tempUAV, nullptr);
	deviceContext->CSSetShaderResources(0, 1, &tempSRV);
	deviceContext->Dispatch(NumGroups, NumGroups, amount);
	deviceContext->CSSetShaderResources(0, 1, &nullSRV);
	deviceContext->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);

	tempUAV->Release();
	tempSRV->Release();
	resource->Release();
	tempResource->Release();

	deviceContext->CSSetShader(nullptr, nullptr, 0);
	deviceContext->RSSetState(nullptr);

	return true;
}

bool HLight::GenerateLightMapsInstanced(Object** objects, UINT amount, ID3D11ShaderResourceView** lightMaps)
{
	ID3D11Device* device = Backend::GetDevice();
	ID3D11DeviceContext* deviceContext = Backend::GetDeviceContext();
	HRESULT hr;

	deviceContext->IASetInputLayout(inpLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//DrawShadowMap(objects, amount);

	lightViewPort.Width = (float)LightMapXY;
	lightViewPort.Height = (float)LightMapXY;
	deviceContext->RSSetViewports(1, &lightViewPort);

	deviceContext->VSSetShader(lightVS, nullptr, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &lightViewProjectBuffer);

	deviceContext->RSSetState(noCullingRS);

	deviceContext->PSSetShader(lightPS, nullptr, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightDataBuffer);
	deviceContext->PSSetShaderResources(0, 1, &shadowMapSRV);

	deviceContext->CSSetShader(lightCS, nullptr, 0);

	D3D11_TEXTURE2D_DESC texDesc{};
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

	FillDescriptions(amount, &texDesc, &rtvDesc, &srvDesc, &uavDesc);

	/*
		Reading a UAV connected to resource with format (R8_UNORM) resulted in the following error:

		D3D11 ERROR: ID3D11DeviceContext::Dispatch:
		The Unordered Access View (UAV) in slot 0 of the Compute Shader unit has the Format (R8_UNORM).
		This format does not support being read from a shader as as UAV.
		This mismatch is invalid if the shader actually uses the view (e.g. it is not skipped due to shader code branching).
		It was unfortunately not possible to have all hardware implementations support reading this format as a UAV,
		despite that the format can written to as a UAV. If the shader only needs to perform reads but not writes to this resource,
		consider using a Shader Resource View instead of a UAV.  [ EXECUTION ERROR #2097381: DEVICE_UNORDEREDACCESSVIEW_FORMAT_LD_UNSUPPORTED]

		Old computer can't read the format (R8_UNORM) in UAVs.
		Solution was to copy the resource to a temporary SRV and read from that.
	*/

	ID3D11Texture2D* tempResource{};
	hr = device->CreateTexture2D(&texDesc, nullptr, &tempResource);
	if (FAILED(hr))
		return false;

	ID3D11Texture2D* resource{};
	hr = device->CreateTexture2D(&texDesc, nullptr, &resource);
	if (FAILED(hr))
		return false;

	ID3D11ShaderResourceView* tempSRV{};

	ID3D11RenderTargetView* tempRTV{};
	ID3D11UnorderedAccessView* tempUAV{};

	ID3D11RenderTargetView* nullRTV{};
	ID3D11UnorderedAccessView* nullUAV{};
	ID3D11ShaderResourceView* nullSRV{};

	hr = device->CreateShaderResourceView(resource, nullptr, lightMaps);
	if (FAILED(hr))
		return false;

	const UINT NumGroups = LightMapXY / LightMapCSThreadXY;
	bool succeeded = true;
	for (UINT i = 0; i < amount; i++)
	{
		rtvDesc.Texture2DArray.FirstArraySlice = i;

		hr = device->CreateRenderTargetView(resource, &rtvDesc, &tempRTV);
		if (FAILED(hr))
		{
			succeeded = false;
			continue;
		}

		deviceContext->OMSetRenderTargets(1, &tempRTV, nullptr);
		objects[i]->DrawGeometry();

		tempRTV->Release();
	}

	deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
	deviceContext->CopyResource(tempResource, resource);

	srvDesc.Texture2DArray.ArraySize = amount;
	srvDesc.Texture2DArray.FirstArraySlice = 0;

	hr = device->CreateShaderResourceView(tempResource, &srvDesc, &tempSRV);
	if (FAILED(hr))
		return false;

	hr = device->CreateUnorderedAccessView(resource, &uavDesc, &tempUAV);
	if (FAILED(hr))
		return false;

	deviceContext->CSSetUnorderedAccessViews(0, 1, &tempUAV, nullptr);
	deviceContext->CSSetShaderResources(0, 1, &tempSRV);
	deviceContext->Dispatch(NumGroups, NumGroups, amount);
	deviceContext->CSSetShaderResources(0, 1, &nullSRV);
	deviceContext->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);

	tempUAV->Release();
	tempSRV->Release();
	resource->Release();
	tempResource->Release();

	deviceContext->CSSetShader(nullptr, nullptr, 0);
	deviceContext->RSSetState(nullptr);

	return true;
}

void HLight::DrawShadowMap(Object** objects, UINT amount)
{
	ID3D11DeviceContext* deviceContext = Backend::GetDeviceContext();
	ID3D11RenderTargetView* nullRTV{};

	lightViewPort.Width = (float)ShadowMapXY;
	lightViewPort.Height = (float)ShadowMapXY;
	deviceContext->RSSetViewports(1, &lightViewPort);

	deviceContext->VSSetShader(vs, nullptr, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &lightViewProjectBuffer);

	deviceContext->RSSetState(frontFaceCullingRS);

	deviceContext->PSSetShader(nullptr, nullptr, 0);
	deviceContext->OMSetRenderTargets(1, &nullRTV, shadowMapDSV);

	for (UINT i = 0; i < amount; i++)
		objects[i]->DrawGeometry();
	
	deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
}

void HLight::FillDescriptions(UINT numObjects, D3D11_TEXTURE2D_DESC* texDesc, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	texDesc->Width = LightMapXY;
	texDesc->Height = LightMapXY;
	texDesc->MipLevels = 1;
	texDesc->ArraySize = numObjects;
	texDesc->Format = DXGI_FORMAT_R8_UNORM;
	texDesc->SampleDesc.Count = 1;
	texDesc->SampleDesc.Quality = 0;
	texDesc->Usage = D3D11_USAGE_DEFAULT;
	texDesc->BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
	texDesc->CPUAccessFlags = 0;
	texDesc->MiscFlags = 0;

	rtvDesc->Format = texDesc->Format;
	rtvDesc->ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc->Texture2DArray.ArraySize = 1;
	rtvDesc->Texture2DArray.MipSlice = 0;

	srvDesc->Format = texDesc->Format;
	srvDesc->ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc->Texture2DArray.ArraySize = 1;
	srvDesc->Texture2DArray.MipLevels = 1;
	srvDesc->Texture2DArray.MostDetailedMip = 0;

	uavDesc->Format = texDesc->Format;
	uavDesc->ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc->Texture2DArray.ArraySize = numObjects;
	uavDesc->Texture2DArray.MipSlice = 0;
	uavDesc->Texture2DArray.FirstArraySlice = 0;
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
	texDesc.Height = ShadowMapXY;
	texDesc.Width = ShadowMapXY;
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

	Backend::GetDeviceContext()->ClearDepthStencilView(shadowMapDSV, D3D11_CLEAR_DEPTH, 1.f, 0);

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

	succeeded = Backend::LoadShader(Backend::ShaderPath + "LightMapCS.cso", &shaderData);
	if (!succeeded)
		return false;
	
	hr = Backend::GetDevice()->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &lightCS);
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

	SUN.strength = 10.f;
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
