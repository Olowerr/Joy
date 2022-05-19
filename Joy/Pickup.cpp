#include "Pickup.h"

Pickup::Pickup(Character& joy)
	: pickupVS(nullptr), pickupPS(nullptr), pickupTransSRV(nullptr), pickupsRendered(0), joy(joy)
{
	pickupObjs.reserve(66);

	// Loads VS & PS
	bool hasShaders = LoadPickupShader();
	assert(hasShaders);
}

bool Pickup::isHit()
{
	for (unsigned int i = 0; i < pickupsRendered; i++)
	{
		if (joy.GetBoundingBox(0).Intersects(pickupObjs.at(i)->GetBoundingBox(0)))
		{
			std::swap(pickupObjs.at(i), pickupObjs.at(pickupsRendered - 1));
			pickupsRendered--;
			return true;
		}
	}
	return false;
}

UINT Pickup::getPickupsCaught()
{
	return (pickupObjs.size() - pickupsRendered);
}

void Pickup::AddObject(float pX_in, float pY_in, float pZ_in)
{
	pickupObjs.emplace_back(new Object(pickupMesh, true));
	Object::DropPtr(pickupObjs.back());

	pickupObjs.back()->Translate(pX_in, pY_in, pZ_in);
	pickupsRendered++;
}

void Pickup::FetchPickupMesh(TempMeshStorage& meshStorage_in)
{
	pickupMesh = meshStorage_in.GetMeshByName("PMesh"); 
}

void Pickup::UpdateMatrices()
{

	for (unsigned int i = 0; i < pickupsRendered; i++)
	{
		pickupObjs.at(i)->Rotate(0.0f, 2.0f * Backend::GetDeltaTime(), 0.0f);
		matrices[i] = pickupObjs[i]->GetWorldMatrix();
	}

	Backend::UpdateBuffer(matrixSBuffer, matrices, sizeof(DirectX::XMFLOAT4X4) * pickupObjs.size());
}

bool Pickup::CreateSRV_CreateMatrixSB()
{
	HRESULT hr;
	matrices = new DirectX::XMFLOAT4X4[pickupObjs.size()];

	/* ==TRANSFORM_BUFFER======================================================= */
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(DirectX::XMFLOAT4X4) * pickupObjs.size();
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = sizeof(DirectX::XMFLOAT4X4);

	D3D11_SUBRESOURCE_DATA inData{};
	inData.pSysMem = &matrices;
	inData.SysMemPitch = inData.SysMemSlicePitch = 0;

	hr = Backend::GetDevice()->CreateBuffer(&desc, &inData, &matrixSBuffer);

	if (FAILED(hr))
		return false;

	/* ==TRANSFORM_SHADER_RESOURCE_VIEW======================================================= */
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = sizeof(DirectX::XMFLOAT4X4);
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = pickupObjs.size();

	hr = Backend::GetDevice()->CreateShaderResourceView(matrixSBuffer, &srvDesc, &pickupTransSRV);

	if (FAILED(hr))
		return false;

	return true;
}


bool Pickup::LoadPickupShader()
{
	std::string shaderData;

	if (!Backend::LoadShader(Backend::ShaderPath + "PickupVS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &pickupVS)))
		return false;

	if (!Backend::LoadShader(Backend::ShaderPath + "PickupPS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pickupPS)))
		return false;

	return true;
}

void Pickup::ShutDown()
{
	delete[] this->matrices;
	pickupVS->Release();
	pickupPS->Release();

	matrixSBuffer->Release();
	pickupTransSRV->Release();

	for (unsigned int i = 0; i < pickupObjs.size(); i++)
	{
		pickupObjs.at(i)->Shutdown();
		delete pickupObjs.at(i);
	}
	pickupObjs.clear();
}

void Pickup::DrawPickupInstances(Camera* cam_in)
{
	ID3D11DeviceContext* dc = Backend::GetDeviceContext();

	dc->IASetInputLayout(Backend::GetShaderStorage().objectInputLayout);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dc->IASetVertexBuffers(0, 1, &pickupMesh->vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
	dc->IASetIndexBuffer(pickupMesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0); // TODO : Add Index Buffer

	dc->VSSetShader(pickupVS, nullptr, 0);
	dc->VSSetConstantBuffers(1, 1, cam_in->GetMatrixBuffer());
	dc->VSSetShaderResources(0, 1, &pickupTransSRV);

	dc->PSSetShaderResources(0, 1, &pickupMesh->diffuseTextureSRV);
	dc->PSSetShader(pickupPS, nullptr, 0);

	dc->RSSetViewports(1, &Backend::GetDefaultViewport());
	dc->OMSetRenderTargets(1, Backend::GetMainRTV(), *Backend::GetStandardDSV());

	dc->DrawIndexedInstanced(pickupObjs.at(0)->GetMesh()->indexCount, pickupsRendered, 0, 0, 0);

	HObject::GetInstance().Draw(pickupObjs.at(0), cam_in, false, true, 0);

	dc = nullptr;
}