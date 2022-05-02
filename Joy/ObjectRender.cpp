#include "ObjectRender.h"

ObjectRender::ObjectRender()
	:objVS(nullptr), objPS(nullptr), inpLayout(nullptr), bbRTV(nullptr)
{
	bool succeeded = false;

	succeeded = LoadShaders();
	assert(succeeded);

	bbRTV = Backend::GetBackBufferRTV();
}

void ObjectRender::Shutdown()
{
	sampler->Release();
	inpLayout->Release();
	objVS->Release();
	objPS->Release();
	objInstanceVS->Release();
	decalShadow.Shutdown();
}

void ObjectRender::Clear()
{
	for (InstancedObject& inst : instances)
		inst.Shutdown();
	instances.clear();
	objects.clear();
}

void ObjectRender::CreateSamplerState()
{
	HRESULT hr;

	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;

	hr = Backend::GetDevice()->CreateSamplerState(&samplerDesc, &sampler);
	assert(SUCCEEDED(hr));
}

void ObjectRender::SetActiveCamera(Camera* camera)
{
	Backend::GetDeviceContext()->VSSetConstantBuffers(1, 1, camera->GetMatrixBuffer());
	activeCamera = camera;
}

bool ObjectRender::LoadShaders()
{
	std::string shaderData;

	// Normal Shaders
	if (!Backend::LoadShader(Backend::ShaderPath + "ObjVS.cso", &shaderData))
		return false;

	if (!CreateInputLayout(shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &objVS)))
		return false;

	if (!Backend::LoadShader(Backend::ShaderPath + "ObjPS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &objPS)))
		return false;


	// Instanced Shaders
	if (!Backend::LoadShader(Backend::ShaderPath + "ObjInstanceVS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &objInstanceVS)))
		return false;

	CreateSamplerState(); // << temporary
	Backend::GetDeviceContext()->PSSetSamplers(0, 1, &sampler);
	return true;
}

bool ObjectRender::CreateInputLayout(const std::string& shaderData)
{

	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = Backend::GetDevice()->CreateInputLayout(inputDesc, 3, shaderData.c_str(), shaderData.length(), &inpLayout);

	return SUCCEEDED(hr);
}

void ObjectRender::AddObject(Object* obj)
{
	objects.emplace_back(obj);
}

void ObjectRender::DrawAll()
{
	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

	devContext->IASetInputLayout(inpLayout);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devContext->VSSetShader(objVS, nullptr, 0);
	
	decalShadow.DrawDecalShadowDepth(objects, instances, objects[0]->GetPosition());

	Backend::GetDeviceContext()->VSSetConstantBuffers(1, 1, activeCamera->GetMatrixBuffer());

	devContext->RSSetViewports(1, &Backend::GetDefaultViewport());

	devContext->PSSetShader(decalShadow.GetDecalPS(), nullptr, 0);
	devContext->PSSetConstantBuffers(0, 1, &decalShadow.GetDecalDCBuff());
	devContext->PSSetConstantBuffers(1, 1, &decalShadow.GetDecalCamDCBuff());
	devContext->PSSetShaderResources(1, 1, &decalShadow.GetDecalSRV());

	devContext->OMSetRenderTargets(1, bbRTV, nullptr);
	
	for (Object* obj : objects)
		obj->Draw();

	devContext->VSSetShader(objInstanceVS, nullptr, 0);

	for (InstancedObject& inst : instances)
	{
		devContext->IASetVertexBuffers(0, 1, &inst.vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
		devContext->VSSetShaderResources(0, 1, &inst.transformSRV);

		devContext->DrawInstanced(inst.indexCount, inst.instanceCount, 0, 0);
	}

	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	devContext->PSSetShaderResources(1, 1, nullSRV);
	devContext->OMSetRenderTargets(0, nullptr, nullptr);
}

bool ObjectRender::GiveInstancedObjects(Object* obj, const UINT amount)
{
	HRESULT hr;

	DirectX::XMFLOAT4X4* matrices = new DirectX::XMFLOAT4X4[amount];

	for (UINT i = 0; i < amount; i++)
	{
		matrices[i] = obj[i].GetWorldMatrix();

		// might get moved with lightmaps
		obj[i].Shutdown(); 
	}

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(DirectX::XMFLOAT4X4) * amount;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = sizeof(DirectX::XMFLOAT4X4);
	D3D11_SUBRESOURCE_DATA inData{};
	inData.pSysMem = matrices;
	inData.SysMemPitch = inData.SysMemSlicePitch = 0;

	ID3D11Buffer* resource;
	hr = Backend::GetDevice()->CreateBuffer(&desc, &inData, &resource);
	delete[] matrices;

	if (FAILED(hr))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = sizeof(DirectX::XMFLOAT4X4);
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = amount;

	ID3D11ShaderResourceView* tempSRV;
	hr = Backend::GetDevice()->CreateShaderResourceView(resource, &srvDesc, &tempSRV);
	resource->Release();

	if (FAILED(hr))
		return false;

	instances.emplace_back();

	instances.back().instanceCount = amount;
	instances.back().indexCount = obj[0].GetMesh()->vertexCount;
	instances.back().vertexBuffer = obj[0].GetMesh()->vertexBuffer;
	instances.back().transformSRV = tempSRV;
	instances.back().mtl = obj[0].GetMesh()->diffuseTextureSRV;

	return true;
}

ID3D11InputLayout* ObjectRender::GetObjectInputLayout()
{
	return inpLayout;
}

ID3D11VertexShader* ObjectRender::GetObjectVS()
{
	return objVS;
}
