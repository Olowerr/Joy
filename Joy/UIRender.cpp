#include "UIRender.h"

UIRenderer::UIRenderer()
	:UI_IL(nullptr), UI_VS(nullptr), UI_RS(nullptr), UI_PS(nullptr), quadBuffer(nullptr)
	, projection(nullptr), spriteBatch(Backend::GetDeviceContext())
{
	HRESULT hr;
	bool succeeded = false;
	std::string shaderData;

	ID3D11Device* device = Backend::GetDevice();


	succeeded = Backend::LoadShader(Backend::ShaderPath + "UI_VS.cso", &shaderData);
	assert(succeeded);

	D3D11_INPUT_ELEMENT_DESC inputDesc;
	inputDesc.SemanticName = "POSITION";
	inputDesc.SemanticIndex = 0;
	inputDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	inputDesc.InputSlot = 0;
	inputDesc.AlignedByteOffset = 0;
	inputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDesc.InstanceDataStepRate = 0;

	hr = device->CreateInputLayout(&inputDesc, 1, shaderData.c_str(), shaderData.length(), &UI_IL);
	assert(SUCCEEDED(hr));

	hr = device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &UI_VS);
	assert(SUCCEEDED(hr));


	D3D11_RASTERIZER_DESC rsDesc{};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;

	hr = device->CreateRasterizerState(&rsDesc, &UI_RS);
	assert(SUCCEEDED(hr));


	succeeded = Backend::LoadShader(Backend::ShaderPath + "UI_PS.cso", &shaderData);
	assert(succeeded);

	hr = device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &UI_PS);
	assert(SUCCEEDED(hr));


	using namespace DirectX;
	XMFLOAT4X4 matrix;
	float width = (float)Backend::GetWindowWidth();
	float height = (float)Backend::GetWindowHeight();
	XMStoreFloat4x4(&matrix, XMMatrixTranspose(XMMatrixOrthographicOffCenterLH(0.f, width, -height + 1.f, 1.f, 0.1f, 1.f)));
	hr = Backend::CreateConstCBuffer(&projection, &matrix, sizeof(XMFLOAT4X4));
	assert(SUCCEEDED(hr));


	DirectX::XMFLOAT2 quadPos[4] =
	{
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{0.0f, -1.0f},
		{1.0f, -1.0f}
	};
	hr = Backend::CreateVertexBuffer(&quadBuffer, quadPos, sizeof(quadPos));
	assert(SUCCEEDED(hr));
}

void UIRenderer::Shutdown()
{
	projection->Release();
	quadBuffer->Release();
	UI_IL->Release();
	UI_VS->Release();
	UI_RS->Release();
	UI_PS->Release();
}

void UIRenderer::Clear()
{
	sprites.clear();
	texts.clear();
}

void UIRenderer::Add(Sprite* sprite)
{
	sprites.emplace_back(sprite);
}

void UIRenderer::Add(Text* text)
{
	texts.emplace_back(text);
	text->Initiate(&spriteBatch, fontPath);
}

void UIRenderer::Draw()
{
	static const UINT stride = sizeof(DirectX::XMFLOAT2);
	static const UINT offset = 0;

	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();
	devContext->RSSetViewports(1, &Backend::GetDefaultViewport());

	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	devContext->IASetInputLayout(UI_IL);
	devContext->IASetVertexBuffers(0, 1, &quadBuffer, &stride, &offset);

	devContext->VSSetShader(UI_VS, nullptr, 0);
	devContext->VSSetConstantBuffers(1, 1, &projection);

	devContext->RSSetState(UI_RS);

	devContext->PSSetShader(UI_PS, nullptr, 0);

	devContext->OMSetRenderTargets(1, Backend::GetMainRTV(), nullptr);

	for (Sprite* element : sprites)
		element->Draw();

	devContext->RSSetState(nullptr);


	spriteBatch.Begin();
	
	for (Text* text : texts)
		text->Draw();
	
	spriteBatch.End();

	Backend::GetDeviceContext()->OMSetBlendState(nullptr, nullptr, 1);
	Backend::GetDeviceContext()->OMSetDepthStencilState(nullptr, 0);
}

void UIRenderer::DisableAll()
{
	for (Sprite* element : sprites)
		element->SetActive(false);

	for (Text* text : texts)
		text->SetActive(false);
}

void UIRenderer::EnableAll()
{
	for (Sprite* element : sprites)
		element->SetActive(true);

	for (Text* text : texts)
		text->SetActive(true);
}
