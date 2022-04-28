#include "Pickup.h"

Pickup::Pickup(TempMeshStorage& meshStorage, UINT points_in)
	: pickupVS(nullptr), pickupPS(nullptr), pickupIL(nullptr), pickupTransSRV(nullptr),
		pickupsRendered(0), charBB(DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
		points(points_in)
{
	// Catch the pickup mesh and store it.
	meshStorage.LoadAll();
	pickupMesh = meshStorage.GetMesh(0);
	
	// Reserve space.
	pickupObjs.reserve(66);
	itemsBB.reserve(66);  // NOTE: To replace.

	// Loads VS & PS, Sets IA.
	bool hasShaders = false;
	hasShaders = LoadPickupShader();
	assert(hasShaders);


// TEMP : Camera will be fed from Camera Class.
	float aspect = (float)Backend::GetWindowWidth() / (float)Backend::GetWindowHeight();
	using namespace DirectX;
	XMFLOAT4X4 matri;
	XMMATRIX view = XMMatrixLookAtLH(XMVectorSet(6.f, 3.f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f));
	XMMATRIX proj = XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, aspect, 0.1f, 100.f);
	XMStoreFloat4x4(&matri, XMMatrixTranspose(view * proj));
	Backend::CreateConstCBuffer(&pickupCam, &matri, 64);
}

void Pickup::isHit()
{
	for (unsigned int i = 0; i < pickupsRendered; i++)
	{
		if (hitItem(charBB, itemsBB[i]))
		{
			std::swap(pickupObjs.at(i), pickupObjs.at(pickupsRendered - 1));
			pickupsRendered--;
		}	
	}
}

UINT Pickup::getPoints()
{
	return points;
}

UINT Pickup::getAmountHitPickups()
{
	return pickupsRendered - pickupObjs.size();
}

void Pickup::AddObject(float pX_in, float pY_in, float pZ_in)
{
	pickupObjs.emplace_back(new Object(pickupMesh));
	pickupObjs.back()->Translate(pX_in, pY_in, pZ_in);
	itemsBB.emplace_back(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)); // TODO : Add support for actual Boundign boxes.
	pickupsRendered++;
}

void Pickup::UpdateMatrices()
{

	for (unsigned int i = 0; i < pickupsRendered; i++)
	{ 
		pickupObjs.at(i)->Rotate(0.0f, 2.0f * Backend::GetDeltaTime(), 0.0f);
		matrices[i] = pickupObjs[i]->GetWorldMatrix();
	}

	Backend::UpdateBuffer(matrixCBuffer, matrices, sizeof(DirectX::XMFLOAT4X4)*pickupObjs.size());
}

bool Pickup::CreateInputLayout(const std::string& shaderData)
{
	D3D11_INPUT_ELEMENT_DESC iLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HRESULT hr = Backend::GetDevice()->CreateInputLayout(iLayout, 3, shaderData.c_str(), shaderData.length(), &pickupIL);
	return SUCCEEDED(hr);
}

bool Pickup::CreateSRV_CreateMatrixCB()
{
	HRESULT hr;
	matrices = new DirectX::XMFLOAT4X4[pickupObjs.size()];

/* ==TRANSFORM BUFFER======================================================= */
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

	hr = Backend::GetDevice()->CreateBuffer(&desc, &inData, &matrixCBuffer);

	if (FAILED(hr))
		return false;

/* ==TRANSFORM_SHADER_RESOURCE_VIEW======================================================= */
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = sizeof(DirectX::XMFLOAT4X4);
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = pickupObjs.size();

	hr = Backend::GetDevice()->CreateShaderResourceView(matrixCBuffer, &srvDesc, &pickupTransSRV);

	if (FAILED(hr))
		return false;


	return true;
}


bool Pickup::LoadPickupShader()
{
	std::string shaderData;

	if (!Backend::LoadShader(Backend::ShaderPath + "PickupVS.cso", &shaderData))
		return false;

	if (!CreateInputLayout(shaderData))
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
	pickupIL->Release();
	pickupVS->Release();
	pickupPS->Release();

	pickupCam->Release();
	matrixCBuffer->Release();
	
	pickupTransSRV->Release();


	
	for (unsigned int i = 0; i < pickupObjs.size(); i++)
	{
		pickupObjs.at(i)->Shutdown();
		delete pickupObjs.at(i);
	}

	pickupObjs.clear();
	itemsBB.clear();
}

void Pickup::DrawPickupInstances()
{
	ID3D11DeviceContext* dc = Backend::GetDeviceContext();
	
	dc->IASetInputLayout(pickupIL);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dc->IASetVertexBuffers(0, 1, &pickupMesh->vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
	
	dc->VSSetShader(pickupVS, nullptr, 0);
	dc->VSSetConstantBuffers(0, 1, &pickupCam);
	dc->VSSetShaderResources(0, 1, &pickupTransSRV);

	dc->PSSetShader(pickupPS, nullptr, 0);

	dc->RSSetViewports(1, &Backend::GetDefaultViewport());
	dc->OMSetRenderTargets(1, Backend::GetBackBufferRTV(), nullptr);



	dc->DrawInstanced(pickupObjs[0]->GetMesh()->vertexCount, pickupsRendered, 0, 0);	
	
	dc = nullptr;
}

