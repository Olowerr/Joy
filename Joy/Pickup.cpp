#include "Pickup.h"

Pickup::Pickup(Mesh* mesh_in, int points_in, const int itemsInScene_in)
	: Object(mesh_in), pickupObj(mesh_in), itemsInScene(itemsInScene_in),
	pickupVS(nullptr), pickupPS(nullptr), pickupIL(nullptr), pickupRTV(nullptr)
{
/* ========================================================= */
	// == DX SPECIFICS ==

	bool hasLoadedShaders = false;
	hasLoadedShaders = LoadPickupShader();
	assert(hasLoadedShaders);

	pickupRTV = Backend::GetBackBufferRTV();

	// == TEMP CAM SPECIFICS ==
		//float aspect = (float)Backend::GetWindowWidth() / (float)Backend::GetWindowHeight();
		//using namespace DirectX;
		//XMFLOAT4X4 matri;
		//XMMATRIX view = XMMatrixLookAtLH(XMVectorSet(6.f, 3.f, 0.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f));
		//XMMATRIX proj = XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, aspect, 0.1f, 100.f);
		//XMStoreFloat4x4(&matri, XMMatrixTranspose(view * proj));

		//Backend::CreateConstCBuffer(&pickupCam, &matri, 64);

/* ========================================================= */

	// == PICKUP SPECIFICS ==
	this->points = points_in;

	this->isRendered.reserve(itemsInScene);
	this->itemsBB.reserve(itemsInScene);
	this->instanceVec.reserve(itemsInScene);


	for (unsigned int i = 0; i < isRendered.size(); i++)
		this->isRendered[i] = true;

/* ========================================================= */
}

void Pickup::isHit()
{

	for (unsigned int i = 0; i < isRendered.size(); i++)
	{
		if (hitItem(charBB, itemsBB[i]))
		{
			this->isRendered[i] = false;
		}
	}
}

bool Pickup::get_IsElementRendered(int itemElement_in)
{
	return this->isRendered[itemElement_in];
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
	if (!CreateInputLayout(shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pickupPS)))
		return false;
	
	return true;
}

bool Pickup::GiveInstancedObjects(Object* obj, const UINT amount)
{
	HRESULT hr;

	DirectX::XMFLOAT4X4* matrices = new DirectX::XMFLOAT4X4[amount];

	for (UINT i = 0; i < amount; i++)
	{
		matrices[i] = obj[i].GetWorldMatrix();

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

	instanceVec.emplace_back();

	instanceVec.back().instancePickupCount = amount;
	instanceVec.back().indexPickupCount = obj[0].GetMesh()->vertexCount;
	instanceVec.back().instancedPickupVB = obj[0].GetMesh()->vertexBuffer;
	instanceVec.back().transformPickupSRV = tempSRV;
	instanceVec.back().mtlPickup = obj[0].GetMesh()->diffuseTextureSRV;

	return true;
}

bool Pickup::CreateInputLayout(const std::string &shaderData)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = Backend::GetDevice()->CreateInputLayout(inputDesc, 3, shaderData.c_str(), shaderData.length(), &pickupIL);

	return false;
}

void Pickup::Clear()
{
	for (InstancedPickups& inst : this->instanceVec)
		inst.Shutdown();
	this->instanceVec.clear();
}

void Pickup::ShutDown()
{
	matrixCBuffer->Release();
	pickupIL->Release();
	pickupVS->Release();
	pickupPS->Release();

	// temp
	pickupCam->Release();
}

void Pickup::drawInstanced()
{


}

void Pickup::updatePositions()
{
	// TODO: Update Buffer

	Backend::UpdateBuffer(matrixCBuffer, nullptr, 2);

}


