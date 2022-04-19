#include "Pickup.h"

Pickup::Pickup(TempMeshStorage& meshStorage, int points_in, const int itemsInScene_in)
	: itemsInScene(itemsInScene_in), pickupVS(nullptr), pickupPS(nullptr), pickupIL(nullptr), pickupRTV(nullptr)
{
	meshStorage.LoadAll();
	
	isRendered.reserve(itemsInScene);
	pickupObjs.reserve(itemsInScene);
	itemsBB.reserve(itemsInScene); 

	pickupMesh = meshStorage.GetMesh(0);
}

void Pickup::isHit()
{
	for (unsigned int i = 0; i < isRendered.size(); i++)
	{
		if (hitItem(charBB, itemsBB[i]))
			this->isRendered[i] = false;
	}
}

bool Pickup::Get_IsElementRendered(int itemElement_in)
{
	return this->isRendered[itemElement_in];
}

void Pickup::AddObject(Object obj_in, float position_in[3])
{
	pickupObjs.emplace_back(obj_in);
	obj_in.SetPosition(position_in[0], position_in[1], position_in[2]);
}

void Pickup::RotateMatrices()
{
	matrices = new DirectX::XMFLOAT4X4[pickupObjs.size()];

	for (unsigned int i = 0; i < pickupObjs.size(); i++)
	{ 
		pickupObjs[i].Rotate(0.0f, 2.0f, 0.0f);
		matrices[i] = pickupObjs[i].GetWorldMatrix();
	}

	//Backend::UpdateBuffer(pickupVS, &matrices);

}

bool Pickup::CreateInputLayout(const std::string& shaderData)
{
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
	if (!CreateInputLayout(shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &pickupPS)))
		return false;
	
	return true;
}

bool Pickup::ShutDown()
{
	delete this->matrices;

	return false;
}

