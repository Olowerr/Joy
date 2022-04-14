#include "Pickup.h"

Pickup::Pickup(TempMeshStorage& meshStorage, int points_in, const int itemsInScene_in)
	: itemsInScene(itemsInScene_in), pickupVS(nullptr), pickupPS(nullptr), pickupIL(nullptr), pickupRTV(nullptr)
{

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

void Pickup::AddPickupObjects(Object* obj, float position[3])
{
	pickupObjs.emplace_back(obj);
	pickupObjs.back().Translate(position[0], position[1], position[2]);

	isRendered.emplace_back(true);
}

void Pickup::UpdateMatrices()
{
	for (Object obj : pickupObjs)
		obj.Rotate(0.0f, 2.0f, 0.0f);
}

bool Pickup::CreateInputLayout(const std::string& shaderData)
{

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

