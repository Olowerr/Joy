#include "ObjectRender.h"

ObjectRender::ObjectRender()
	:bbRTV(nullptr), storage(Backend::GetShaderStorage())
{
	CreateSamplerState(); // << temporary
	Backend::GetDeviceContext()->PSSetSamplers(0, 1, &sampler);

	bbRTV = Backend::GetBackBufferRTV();
}

void ObjectRender::Shutdown()
{
	sampler->Release();

}

void ObjectRender::Clear()
{
	for (InstancedObject& inst : instances)
		inst.Shutdown();
	instances.clear();
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

void ObjectRender::SetMapDivier(MapDivider* sections)
{
	this->sections = sections;
	activeSection = sections->GetActiveSection();
}

void ObjectRender::DrawAll()
{
	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

	devContext->IASetInputLayout(storage.objectInputLayout);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devContext->VSSetShader(storage.objectVS, nullptr, 0);

	Backend::GetDeviceContext()->VSSetConstantBuffers(1, 1, activeCamera->GetMatrixBuffer());

	devContext->RSSetViewports(1, &Backend::GetDefaultViewport());

	devContext->PSSetShader(storage.objectPS, nullptr, 0);

	devContext->OMSetRenderTargets(1, bbRTV, *Backend::GetStandardDSV());
	
	// temp


	for (Object* obj : (*activeSection)->enivormentObjects)
		obj->Draw();

	//devContext->VSSetShader(objInstanceVS, nullptr, 0);

	//for (InstancedObject& inst : instances)
	//{
	//	devContext->IASetVertexBuffers(0, 1, &inst.vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
	//	devContext->VSSetShaderResources(0, 1, &inst.transformSRV);

	//	devContext->DrawInstanced(inst.indexCount, inst.instanceCount, 0, 0);
	//}

	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	devContext->PSSetShaderResources(1, 1, nullSRV);
	devContext->OMSetRenderTargets(0, nullptr, nullptr);
}

void ObjectRender::DrawCharacter(Character& character)
{
	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

	devContext->IASetInputLayout(storage.objectInputLayout);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devContext->VSSetShader(storage.objectVS, nullptr, 0);
	Backend::GetDeviceContext()->VSSetConstantBuffers(1, 1, activeCamera->GetMatrixBuffer());

	devContext->RSSetViewports(1, &Backend::GetDefaultViewport());

	devContext->PSSetShader(storage.JoyPS, nullptr, 0);

	devContext->OMSetRenderTargets(1, bbRTV, *Backend::GetStandardDSV());

	character.Draw();
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
