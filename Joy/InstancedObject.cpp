#include "InstancedObject.h"

std::vector<InstancedObject*> InstancedObject::levelInstanced;
std::vector<InstancedObject*> InstancedObject::enviormentInstanced;

InstancedObject::InstancedObject(UINT capacity)
	:Capacity(capacity), instanceCount(0), transformSRV(nullptr), mesh(nullptr), lightMapsSRV(nullptr)
{
	ppObjects = new Object * [Capacity];
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
	if (instanceCount == 0)
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
	devContext->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, Mesh::Offset);
	devContext->VSSetShaderResources(0, 1, &transformSRV);
	devContext->PSSetShaderResources(0, 1, &mesh->diffuseTextureSRV);
	devContext->PSSetShaderResources(2, 1, &lightMapsSRV);

	if (mesh->indexBuffer)
		devContext->DrawIndexedInstanced(mesh->indexCount, instanceCount, 0, 0, 0);
	else
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

void InstancedObject::Generate(TempMeshStorage& meshStorage, const std::vector<Object*>& objects, std::vector<InstancedObject*>& target)
{
	std::vector<Object*> tempObjects;
	tempObjects.reserve(10);
	for (size_t i = 0; i < meshStorage.GetMeshCount(); ++i)
	{
		tempObjects.clear();

		if (meshStorage.GetMesh(i)->name == "Grass_garden_platform3")
			int q = 0;

		for (size_t k = 0; k < objects.size(); ++k)
		{
			if (objects[k]->GetMesh() == meshStorage.GetMesh(i) && objects[k]->GetIsImmutable())
				tempObjects.emplace_back(objects[k]);
		}

		if (tempObjects.size() > 1)
		{
			target.emplace_back(new InstancedObject(tempObjects.size()));
			for (Object* object : tempObjects)
				target.back()->AddObject(object);

			target.back()->Finalize();
		}
	}
}

void InstancedObject::DestroyInstancedObjects()
{
	for (InstancedObject* inst : levelInstanced)
	{
		inst->Shutdown();
		delete inst;
	}

	for (InstancedObject* inst : enviormentInstanced)
	{
		inst->Shutdown();
		delete inst;
	}
}

bool InstancedObject::CreateInstancedObjects(TempMeshStorage& meshStorage, MapDivider& sections, HLight& hLight)
{
	const std::vector<Object*>& levelObjects = Object::GetLevelObjects();
	const std::vector<Object*>& enviormentObjects = Object::GetEnviormentObjects();

	Generate(meshStorage, levelObjects, levelInstanced);
	Generate(meshStorage, enviormentObjects, enviormentInstanced);

	for (InstancedObject* inst : levelInstanced)
		hLight.GenerateLightMapsInstanced(sections, inst->GetObjects(), inst->GetNumObjects(), inst->GetLightMaps());

	for (InstancedObject* inst : enviormentInstanced)
		hLight.GenerateLightMapsInstanced(sections, inst->GetObjects(), inst->GetNumObjects(), inst->GetLightMaps());

	return true;
}

const std::vector<InstancedObject*>& InstancedObject::GetLevelInstancedObjects()
{
	return levelInstanced;
}

const std::vector<InstancedObject*>& InstancedObject::GetEnviormentInstancedObjects()
{
	return enviormentInstanced;
}
