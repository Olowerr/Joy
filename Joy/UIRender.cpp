#include "UIRender.h"

void UIRender::Initiate()
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


}

void UIRender::Shutdown()
{
	quadBuffer->Release();
	UI_IL->Release();
	UI_VS->Release();
	UI_RS->Release();
	UI_PS->Release();
}

void UIRender::Clear()
{
	elements.clear();
}

void UIRender::Add(UIElement* element)
{
	elements.emplace_back(element);
}

void UIRender::Draw()
{
	ID3D11DeviceContext* devContext = Backend::GetDeviceContext();

	devContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	devContext->IASetInputLayout(UI_IL);

	devContext->VSSetShader(UI_VS, nullptr, 0);

	devContext->RSSetState(UI_RS);

	devContext->PSSetShader(UI_PS, nullptr, 0);

	for (UIElement* element : elements)
		element->Draw();


	devContext->RSSetState(nullptr);
}
