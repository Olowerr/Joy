#include "ObjectRender.h"

ObjectRender::ObjectRender()
	:bbRTV(nullptr), storage(Backend::GetShaderStorage())
	, enviormentInstanced(InstancedObject::GetEnviormentInstancedObjects())
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

	devContext->VSSetConstantBuffers(1, 1, activeCamera->GetMatrixBuffer());

	devContext->RSSetViewports(1, &Backend::GetDefaultViewport());

	devContext->PSSetShader(storage.objectPS, nullptr, 0);

	devContext->OMSetRenderTargets(1, bbRTV, *Backend::GetStandardDSV());
	

	for (Object* obj : Object::GetEnviormentObjects())
	{
		if (!obj->GetIsInstanced())
			obj->Draw();
	}

	devContext->VSSetShader(storage.objectInstancedVS, nullptr, 0);

	for (InstancedObject* inst : enviormentInstanced)
		inst->Draw();
}

void ObjectRender::DrawCharacter(Character& character)
{
	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

	devContext->IASetInputLayout(storage.objectInputLayout);
	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devContext->VSSetShader(storage.objectVS, nullptr, 0);
	devContext->VSSetConstantBuffers(1, 1, activeCamera->GetMatrixBuffer());

	devContext->RSSetViewports(1, &Backend::GetDefaultViewport());

	devContext->PSSetShader(storage.JoyPS, nullptr, 0);

	devContext->OMSetRenderTargets(1, bbRTV, *Backend::GetStandardDSV());

	character.Draw();
}

bool ObjectRender::GiveInstancedObjects(Object* obj, const UINT amount)
{
	

	//instances.emplace_back();

	//instances.back().instanceCount = amount;
	//instances.back().indexCount = obj[0].GetMesh()->indexCount;
	//instances.back().vertexBuffer = obj[0].GetMesh()->vertexBuffer;
	//instances.back().transformSRV = tempSRV;
	//instances.back().mtl = obj[0].GetMesh()->diffuseTextureSRV;

	return true;
}
