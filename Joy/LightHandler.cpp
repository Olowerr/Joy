#include "LightHandler.h"

HLight::HLight()
	:storage(Backend::GetShaderStorage())
	, lightViewPort(), lightVS(nullptr), lightCS(nullptr), noCullingRS(nullptr), lightPS(nullptr), lightDataBuffer(nullptr)
	,shadowMapDSV(nullptr), shadowMapSRV(nullptr), amount(0), lightViewProjectBuffer(0), frontFaceCullingRS(nullptr)
	, SUN(), viewProject()
{
	lightViewPort.TopLeftX = 0.f;
	lightViewPort.TopLeftY = 0.f;
	lightViewPort.MaxDepth = 1.f;
	lightViewPort.MinDepth = 0.f;
}

void HLight::Shutdown()
{
	lightDataBuffer->Release();
}

void HLight::ShutdownTools()
{
	lightVS->Release();
	noCullingRS->Release();
	lightPS->Release();
	lightCS->Release();
	frontFaceCullingRS->Release();

	for (UINT i = 0; i < amount; i++)
	{
		lightViewProjectBuffer[i]->Release();
		shadowMapDSV[i]->Release();
		shadowMapSRV[i]->Release();
	}
	delete[]shadowMapDSV;
	delete[]shadowMapSRV;
	delete[]lightViewProjectBuffer;
}

HLight::~HLight()
{
}

bool HLight::InitiateTools(MapDivider& sections)
{
	amount = sections.GetNumSections();
	bool succeeded = false;

	shadowMapDSV = new ID3D11DepthStencilView * [amount];
	shadowMapSRV = new ID3D11ShaderResourceView * [amount];

	succeeded = InitiateBuffers(sections);
	assert(succeeded);

	succeeded = InitiateShaders();
	assert(succeeded);

	succeeded = InitiateRasterizerStates();
	assert(succeeded);

	succeeded = InitiateShadowMaps();
	assert(succeeded);


	DrawShadowMaps(sections);

	return false;
}

bool HLight::GenerateLightMaps(MapDivider& sections)
{

	ID3D11Device* device = Backend::GetDevice();
	ID3D11DeviceContext* deviceContext = Backend::GetDeviceContext();
	HRESULT hr;

	lightViewPort.Width = (float)LightMapXY;
	lightViewPort.Height = (float)LightMapXY;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(storage.objectInputLayout);

	deviceContext->VSSetShader(lightVS, nullptr, 0);

	deviceContext->RSSetViewports(1, &lightViewPort);
	deviceContext->RSSetState(noCullingRS);

	deviceContext->PSSetShader(lightPS, nullptr, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightDataBuffer);


	ID3D11RenderTargetView* tempRTV{};
	ID3D11RenderTargetView* nullRTV{};
	ID3D11UnorderedAccessView* nullUAV{};
	ID3D11ShaderResourceView* nullSRV{};

	D3D11_TEXTURE2D_DESC texDesc{};
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

	FillDescriptions(&texDesc, &rtvDesc, &srvDesc, &uavDesc);
	const std::vector<Object*>& levelObjs = Object::GetLevelObjects();
	const std::vector<Object*>& enviormentObjs = Object::GetEnviormentObjects();
	/*for (size_t i = 0; i < levelObjs.size(); i++)
		texDesc.ArraySize += !levelObjs[i]->GetIsInstanced();
	for (size_t i = 0; i < enviormentObjs.size(); i++)
		texDesc.ArraySize += !enviormentObjs[i]->GetIsInstanced();*/
	texDesc.ArraySize = levelObjs.size() + enviormentObjs.size(); // temp
	ID3D11Texture2D* resource{};
	hr = device->CreateTexture2D(&texDesc, nullptr, &resource);
	if (FAILED(hr))
		return false;

	UINT textureIndex = 0;
	for (UINT s = 0; s < sections.GetNumSections(); s++)
	{
		deviceContext->PSSetConstantBuffers(1, 1, &lightViewProjectBuffer[s]);
		deviceContext->PSSetShaderResources(0, 1, &shadowMapSRV[s]);
		
		Section& section = sections.GetSections()[s];

		std::vector<Object*>& levelObjects = section.levelObjects;
		std::vector<Object*>& enviormentObjects = section.enivormentObjects;
		
		for (UINT k = 0; k < levelObjects.size(); k++)
		{
			if (levelObjects[k]->GetIsInstanced() || (*levelObjects[k]->GetLightMapSRV()))
				continue;


			srvDesc.Texture2DArray.FirstArraySlice = textureIndex;
			rtvDesc.Texture2DArray.FirstArraySlice = textureIndex;
			textureIndex++;

			hr = device->CreateShaderResourceView(resource, &srvDesc, levelObjects[k]->GetLightMapSRV());
			if (FAILED(hr))
				continue;

			hr = device->CreateRenderTargetView(resource, &rtvDesc, &tempRTV);
			if (FAILED(hr))
			{
				(*levelObjects[k]->GetLightMapSRV())->Release();
				continue;
			}

			deviceContext->OMSetRenderTargets(1, &tempRTV, nullptr);
			levelObjects.at(k)->DrawGeometry();
			tempRTV->Release();
		}

		for (UINT k = 0; k < enviormentObjects.size(); k++)
		{
			if (enviormentObjects[k]->GetIsInstanced())
				continue;

			srvDesc.Texture2DArray.FirstArraySlice = textureIndex;
			rtvDesc.Texture2DArray.FirstArraySlice = textureIndex;
			textureIndex++;

			hr = device->CreateShaderResourceView(resource, &srvDesc, enviormentObjects[k]->GetLightMapSRV());
			if (FAILED(hr))
				continue;

			hr = device->CreateRenderTargetView(resource, &rtvDesc, &tempRTV);
			if (FAILED(hr))
			{
				(*enviormentObjects[k]->GetLightMapSRV())->Release();
				continue;
			}

			enviormentObjects.at(k)->DrawGeometry();
			tempRTV->Release();
		}
	}
	deviceContext->RSSetState(nullptr);

	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	ID3D11Texture2D* tempResource{};
	hr = device->CreateTexture2D(&texDesc, nullptr, &tempResource);
	if (FAILED(hr))
		return false;

	
	deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
	deviceContext->CopyResource(tempResource, resource);

	srvDesc.Texture2DArray.ArraySize = texDesc.ArraySize;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	uavDesc.Texture2DArray.ArraySize = texDesc.ArraySize;
	uavDesc.Texture2DArray.FirstArraySlice = 0;

	ID3D11ShaderResourceView* tempSRV{};
	hr = device->CreateShaderResourceView(tempResource, &srvDesc, &tempSRV);
	if (FAILED(hr))
		return false;

	ID3D11UnorderedAccessView* tempUAV{};
	hr = device->CreateUnorderedAccessView(resource, &uavDesc, &tempUAV);
	if (FAILED(hr))
		return false;

	const UINT NumGroups = LightMapXY / LightMapCSThreadXY;
	
	deviceContext->CSSetShader(lightCS, nullptr, 0);
	deviceContext->CSSetUnorderedAccessViews(0, 1, &tempUAV, nullptr);
	deviceContext->CSSetShaderResources(0, 1, &tempSRV);
	deviceContext->Dispatch(NumGroups, NumGroups, texDesc.ArraySize);

	deviceContext->CSSetShaderResources(0, 1, &nullSRV);
	deviceContext->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
	deviceContext->CSSetShader(nullptr, nullptr, 0);

	tempUAV->Release();
	tempSRV->Release();
	tempResource->Release();
	resource->Release();

	/*  
		Reading a UAV connected to resource with the format (R8_UNORM) resulted in the following error:

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
	

	return true;
}

bool HLight::GenerateLightMapsInstanced(MapDivider& sections, InstancedObject& instststs)
{
	ID3D11Device* device = Backend::GetDevice();
	ID3D11DeviceContext* deviceContext = Backend::GetDeviceContext();
	HRESULT hr;

	lightViewPort.Width = (float)LightMapXY;
	lightViewPort.Height = (float)LightMapXY;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(storage.objectInputLayout);

	deviceContext->VSSetShader(lightVS, nullptr, 0);

	deviceContext->RSSetViewports(1, &lightViewPort);
	deviceContext->RSSetState(noCullingRS);

	deviceContext->PSSetShader(lightPS, nullptr, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightDataBuffer);


	ID3D11RenderTargetView* tempRTV{};
	ID3D11RenderTargetView* nullRTV{};
	ID3D11UnorderedAccessView* nullUAV{};
	ID3D11ShaderResourceView* nullSRV{};

	D3D11_TEXTURE2D_DESC texDesc{};
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};

	const UINT numObjects = instststs.GetNumObjects();
	Object** objects = instststs.GetObjects();

	FillDescriptions(&texDesc, &rtvDesc, &srvDesc, &uavDesc);
	texDesc.ArraySize = numObjects;
	
	ID3D11Texture2D* resource{};
	hr = device->CreateTexture2D(&texDesc, nullptr, &resource);
	if (FAILED(hr))
		return false;

	hr = device->CreateShaderResourceView(resource, nullptr, instststs.GetLightMaps());
	if (FAILED(hr))
	{
		resource->Release();
		return false;
	}

	for (UINT i = 0; i < numObjects; i++)
	{
		for (UINT s = 0; s < sections.GetNumSections(); s++)
		{
			if (objects[i]->GetBoundingBox().Intersects(sections.GetSections()[s].sectionBB))
			{
				rtvDesc.Texture2DArray.FirstArraySlice = i;
				hr = device->CreateRenderTargetView(resource, &rtvDesc, &tempRTV);
				if (SUCCEEDED(hr))
				{
					deviceContext->PSSetShaderResources(0, 1, &shadowMapSRV[s]);
					deviceContext->PSSetConstantBuffers(1, 1, &lightViewProjectBuffer[s]);
					deviceContext->OMSetRenderTargets(1, &tempRTV, nullptr);

					objects[i]->DrawGeometry();
					tempRTV->Release();
				}
				s = sections.GetNumSections();

			}
		}
	}
	
	deviceContext->RSSetState(nullptr);
	deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);

	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	ID3D11Texture2D* tempResource{};
	hr = device->CreateTexture2D(&texDesc, nullptr, &tempResource);
	if (FAILED(hr))
		return false;


	deviceContext->CopyResource(tempResource, resource);

	srvDesc.Texture2DArray.ArraySize = texDesc.ArraySize;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	uavDesc.Texture2DArray.ArraySize = texDesc.ArraySize;
	uavDesc.Texture2DArray.FirstArraySlice = 0;

	ID3D11ShaderResourceView* tempSRV{};
	hr = device->CreateShaderResourceView(tempResource, &srvDesc, &tempSRV);
	if (FAILED(hr))
		return false;

	ID3D11UnorderedAccessView* tempUAV{};
	hr = device->CreateUnorderedAccessView(resource, &uavDesc, &tempUAV);
	if (FAILED(hr))
		return false;

	const UINT NumGroups = LightMapXY / LightMapCSThreadXY;

	deviceContext->CSSetShader(lightCS, nullptr, 0);
	deviceContext->CSSetUnorderedAccessViews(0, 1, &tempUAV, nullptr);
	deviceContext->CSSetShaderResources(0, 1, &tempSRV);
	deviceContext->Dispatch(NumGroups, NumGroups, texDesc.ArraySize);
	deviceContext->CSSetShaderResources(0, 1, &nullSRV);
	deviceContext->CSSetUnorderedAccessViews(0, 1, &nullUAV, nullptr);
	deviceContext->CSSetShader(nullptr, nullptr, 0);

	tempUAV->Release();
	tempSRV->Release();
	tempResource->Release();
	resource->Release();

	/*
		Reading a UAV connected to resource with the format (R8_UNORM) resulted in the following error:

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


	return true;
}

void HLight::DrawShadowMaps(MapDivider& sections)
{
	

	ID3D11DeviceContext* deviceContext = Backend::GetDeviceContext();
	ID3D11RenderTargetView* nullRTV{};

	lightViewPort.Width = (float)ShadowMapXY;
	lightViewPort.Height = (float)ShadowMapXY;

	deviceContext->IASetInputLayout(storage.posOnlyInputLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(storage.posOnlyVS, nullptr, 0);

	deviceContext->RSSetViewports(1, &lightViewPort);
	deviceContext->RSSetState(frontFaceCullingRS);

	deviceContext->PSSetShader(nullptr, nullptr, 0);

	for (UINT i = 0; i < sections.GetNumSections(); i++)
	{
		deviceContext->VSSetConstantBuffers(1, 1, &lightViewProjectBuffer[i]);
		deviceContext->OMSetRenderTargets(1, &nullRTV, shadowMapDSV[i]);

		for (UINT o = 0; o < sections.GetNumSections(); o++)
		{
			std::vector<Object*>& levelObjects = sections.GetSections()[o].levelObjects;
			std::vector<Object*>& enviormentObjects = sections.GetSections()[o].enivormentObjects;

			for (UINT k = 0; k < levelObjects.size(); k++)
				levelObjects.at(k)->DrawGeometry();

			for (UINT k = 0; k < enviormentObjects.size(); k++)
				enviormentObjects.at(k)->DrawGeometry();
		}

	}

	deviceContext->OMSetRenderTargets(1, &nullRTV, nullptr);
}

void HLight::FillDescriptions(D3D11_TEXTURE2D_DESC* texDesc, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc)
{
	texDesc->Width = LightMapXY;
	texDesc->Height = LightMapXY;
	texDesc->MipLevels = 1;
	texDesc->ArraySize = 0;
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
	uavDesc->Texture2DArray.ArraySize = 1;
	uavDesc->Texture2DArray.MipSlice = 0;
}

bool HLight::InitiateShadowMaps()
{
	bool succeded = false;


	ID3D11Device* device = Backend::GetDevice();
	HRESULT hr;

	D3D11_TEXTURE2D_DESC texDesc{};
	texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	texDesc.ArraySize = amount;
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
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.ArraySize = 1;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.MostDetailedMip = 0;

	for (size_t i = 0; i < amount; i++)
	{
		srvDesc.Texture2DArray.FirstArraySlice = i;
		hr = device->CreateShaderResourceView(resource, &srvDesc, &shadowMapSRV[i]);
		if (FAILED(hr))
			continue;
		
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.MipSlice = 0;
	dsvDesc.Texture2DArray.ArraySize = 1;
	dsvDesc.Flags = 0;
	
	for (size_t i = 0; i < amount; i++)
	{
		dsvDesc.Texture2DArray.FirstArraySlice = i;
		hr = device->CreateDepthStencilView(resource, &dsvDesc, &shadowMapDSV[i]);
		if (FAILED(hr))
			continue;

		Backend::GetDeviceContext()->ClearDepthStencilView(shadowMapDSV[i], D3D11_CLEAR_DEPTH, 1.f, 0);
	}

	resource->Release();


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

bool HLight::InitiateBuffers(MapDivider& sections)
{
	bool succeeded = false;
	HRESULT hr;

	using namespace DirectX;
	const FLOAT SUNDISTANCE = 20.f;
	const XMVECTOR SUNDIR = XMVectorSet(-1.f, -1.f, 1.f, 0.f); // From sun to ground
	XMVECTOR SUNPOS{};

	SUN.strength = 2.f;
	XMStoreFloat3(&SUN.direction, -XMVector3Normalize(SUNDIR));

	hr = Backend::CreateDynamicCBuffer(&lightDataBuffer, &SUN, sizeof(DirectionalLight));
	if (FAILED(hr))
		return false;

	lightViewProjectBuffer = new ID3D11Buffer * [sections.GetNumSections()];
	const float xLength = sections.GetSections()[0].sectionBB.Extents.x;
	const float yLength = sections.GetSections()[0].sectionBB.Extents.y;
	const float zLength = sections.GetSections()[0].sectionBB.Extents.z;
	const float viewDimension = std::sqrt(xLength * xLength + yLength * yLength + zLength * zLength) * 1.5f;

	for (UINT i = 0; i < sections.GetNumSections(); i++)
	{
		Section& currentSection = sections.GetSections()[i];

		SUNPOS = XMVectorAdd(
			XMVectorSet(currentSection.sectionBB.Center.x, 0.f, currentSection.sectionBB.Center.z, 0.f),
			XMVectorScale(SUNDIR, -SUNDISTANCE));

		XMStoreFloat4x4(&viewProject, XMMatrixTranspose(
			XMMatrixLookToLH(SUNPOS, SUNDIR, XMVectorSet(0.f, 1.f, 0.f, 0.f)) * 
			XMMatrixOrthographicLH(viewDimension, viewDimension, 0.1f, 100.f)));

		hr = Backend::CreateDynamicCBuffer(&lightViewProjectBuffer[i], &viewProject, sizeof(XMFLOAT4X4));
		if (FAILED(hr))
			continue;
	}

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
