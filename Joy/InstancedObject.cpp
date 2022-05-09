#include "InstancedObject.h"

InstancedObject::InstancedObject(UINT capacity)
	:Capacity(capacity), instanceCount(0), transformSRV(nullptr), mesh(nullptr), lightMapsSRV(nullptr)
{
	ppObjects = new Object*[Capacity];
}

InstancedObject::~InstancedObject()
{
}

void InstancedObject::Shutdown()
{
	transformSRV->Release();
	lightMapsSRV->Release();
	// Don't release mesh, MeshStorage owns it

	delete[] ppObjects;
}

void InstancedObject::AddObject(Object* pObject)
{
	if (instanceCount >= 10)
		return;

	else if (instanceCount == 0)
		mesh = pObject->GetMesh();

	else if (mesh != pObject->GetMesh())
		return;

	pObject->SetIsInstanced(true);
	ppObjects[instanceCount++] = pObject;
}

bool InstancedObject::Finalize()
{
	HRESULT hr;

	DirectX::XMFLOAT4X4* matrices = new DirectX::XMFLOAT4X4[instanceCount];

	for (UINT i = 0; i < instanceCount; i++)
		matrices[i] = ppObjects[i]->GetWorldMatrix();

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(DirectX::XMFLOAT4X4) * instanceCount;
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
	{
		delete[] ppObjects;
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = sizeof(DirectX::XMFLOAT4X4);
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = instanceCount;

	hr = Backend::GetDevice()->CreateShaderResourceView(resource, &srvDesc, &transformSRV);
	resource->Release();

	if (FAILED(hr))
	{
		delete[] ppObjects;
		return false;
	}

	ShrinkToFit();

	return true;
}

ID3D11ShaderResourceView** InstancedObject::GetLightMaps()
{
	return &lightMapsSRV;
}

Object** InstancedObject::GetObjects()
{
	return ppObjects;
}

UINT InstancedObject::GetNumObjects()
{
	return instanceCount;
}

void InstancedObject::Draw()
{
	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

	devContext->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
	//devContext->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, Mesh::Offset);
	devContext->VSSetShaderResources(0, 1, &transformSRV);
	devContext->PSSetShaderResources(0, 1, &mesh->diffuseTextureSRV);

	devContext->DrawInstanced(mesh->indexCount, instanceCount, 0, 0);
}

void InstancedObject::ShrinkToFit()
{
	Object** pTemp = new Object * [instanceCount];
	for (UINT i = 0; i < instanceCount; i++)
		pTemp[i] = ppObjects[i];

	delete[]ppObjects;
	ppObjects = pTemp;
}
