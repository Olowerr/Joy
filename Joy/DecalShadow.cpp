#include "DecalShadow.h"

DecalShadow::DecalShadow()
{
	decalViewPort.TopLeftX = 0.0f;
	decalViewPort.TopLeftY = 0.0f;
	decalViewPort.Width = 1080.0f;
	decalViewPort.Height = 1080.0f;
	decalViewPort.MinDepth = 0.0f;
	decalViewPort.MaxDepth = 1.0f;
	InitiateRasterizerState();
	InitiateDecalDepthBuffer();
}

void DecalShadow::Shutdown()
{
	decalDCBuff->Release();
	decalDSView->Release();
	decalSRV->Release();
	decalCamDCBuff->Release();
	frontFaceCullingRS->Release();
}

bool DecalShadow::InitiateRasterizerState()
{
	HRESULT hr;

	D3D11_RASTERIZER_DESC rsDesc{};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.DepthBias = 0;
	rsDesc.SlopeScaledDepthBias = 0.f;
	rsDesc.DepthBiasClamp = 0.f;
	rsDesc.DepthClipEnable = true;
	rsDesc.ScissorEnable = false;
	rsDesc.MultisampleEnable = false;
	rsDesc.AntialiasedLineEnable = false;

	hr = Backend::GetDevice()->CreateRasterizerState(&rsDesc, &frontFaceCullingRS);
	if (FAILED(hr))
		return false;

	return true;
}

void DecalShadow::CreateCharacterDecal(Character* character)
{
	DirectX::XMFLOAT3 pos = character->GetPosition();

	charPos.x = pos.x;
	charPos.y = pos.y - 2.0f;
	charPos.z = pos.z;
	charPos.w = 1.0f;

	Backend::CreateDynamicCBuffer(&decalDCBuff, &charPos, sizeof(DirectX::XMFLOAT4));
}

void DecalShadow::UpdateCharacterDecal(Character* character)
{
	DirectX::XMFLOAT3 pos = character->GetPosition();

	charPos.x = pos.x;
	charPos.y = pos.y - 2.0f;
	charPos.z = pos.z;
	charPos.w = 1.0f;

	Backend::UpdateBuffer(decalDCBuff, &charPos, sizeof(DirectX::XMFLOAT4));
}

ID3D11Buffer* const* DecalShadow::getDecalBuffer()
{
	return &decalDCBuff;
}

bool DecalShadow::InitiateDecalDepthBuffer()
{
	HRESULT hr;
	ID3D11Device* device = Backend::GetDevice();

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = 1080;
	textureDesc.Height = 1080;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
	stencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	stencilDesc.Flags = 0;
	stencilDesc.Texture2D.MipSlice = 0;
	stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	hr = device->CreateTexture2D(&textureDesc, nullptr, &decalDepthTex);
	if (FAILED(hr))
		return false;

	hr = device->CreateDepthStencilView(decalDepthTex, &stencilDesc, &decalDSView);
	if (FAILED(hr))
	{
		decalDepthTex->Release();
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(decalDepthTex, &srvDesc, &decalSRV);
	if (FAILED(hr))
	{
		decalDepthTex->Release();
		decalDSView->Release();
		return false;
	}

	decalDepthTex->Release();
	return true;
}

ID3D11ShaderResourceView* const* DecalShadow::getDecalSRV()
{
	return &decalSRV;
}

void DecalShadow::CreateDecalDepthCam(Character* character)
{
	DirectX::XMMATRIX viewOrtXMtrx = DirectX::XMMatrixLookToLH({ character->GetPosition().x, character->GetPosition().y - 1.5f,
	character->GetPosition().z }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }) *
		DirectX::XMMatrixOrthographicLH(5.0f, 5.0f, 0.1f, 100.0f);
	DirectX::XMStoreFloat4x4(&viewOrtMtrx, DirectX::XMMatrixTranspose(viewOrtXMtrx));
	Backend::CreateDynamicCBuffer(&decalCamDCBuff, &viewOrtMtrx, sizeof(DirectX::XMFLOAT4X4));
}

void DecalShadow::UpdateDecalDepthCam(Character* character)
{
	DirectX::XMMATRIX viewOrtXMtrx = DirectX::XMMatrixLookToLH({ character->GetPosition().x, character->GetPosition().y - 1.5f,
	character->GetPosition().z }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }) *
		DirectX::XMMatrixOrthographicLH(10.0f, 10.0f, 0.1f, 100.0f);
	DirectX::XMStoreFloat4x4(&viewOrtMtrx, DirectX::XMMatrixTranspose(viewOrtXMtrx));
	Backend::UpdateBuffer(decalCamDCBuff, &viewOrtMtrx, sizeof(DirectX::XMFLOAT4X4));
}

ID3D11Buffer* const* DecalShadow::getDecalCamDCBuff()
{
	return &decalCamDCBuff;
}

void DecalShadow::DrawDecalShadowDepth(std::vector<Object*>& objects, std::vector<InstancedObject>& instancedObjects)
{
	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();
	devContext->ClearDepthStencilView(decalDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	devContext->VSSetConstantBuffers(1, 1, &decalCamDCBuff);

	devContext->RSSetState(frontFaceCullingRS);
	devContext->RSSetViewports(1, &decalViewPort);

	devContext->PSSetShader(nullptr, nullptr, 0);
	devContext->OMSetRenderTargets(0, nullptr, decalDSView);

	for (Object* obj : objects)
		if (obj != objects[0])
		{
			obj->Draw();
		}

	//devContext->VSSetShader(decalInstanceVS, nullptr, 0);

	//for (InstancedObject& inst : instancedObjects)
	//{
	//	devContext->IASetVertexBuffers(0, 1, &inst.vertexBuffer, &Mesh::Stirde, &Mesh::Offset);
	//	devContext->VSSetShaderResources(0, 1, &inst.transformSRV);

	//	devContext->DrawInstanced(inst.indexCount, inst.instanceCount, 0, 0);
	//}

	devContext->OMSetRenderTargets(0, nullptr, nullptr);
	devContext->RSSetState(nullptr);
}

bool DecalShadow::LoadShaders()
{
	std::string shaderData;

	// Normal Shaders
	if (!Backend::LoadShader(Backend::ShaderPath + "DecalPS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &decalPS)))
		return false;

	return true;
}
