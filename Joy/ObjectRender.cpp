#include "ObjectRender.h"

ObjectRender::ObjectRender()
{
}

ObjectRender::~ObjectRender()
{
}

bool ObjectRender::LoadShaders(ID3D11Device* device)
{
	std::string shaderData;
	std::ifstream reader;
	reader.open("../Shaders/ObjVS.cso", std::ios::binary | std::ios::ate);

	if (!reader.is_open())
		return false;

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &objVS)))
		return false;

	vShaderByteCode = shaderData;
	shaderData.clear();
	reader.close();
	reader.open("../Shaders/ObjPX.cso", std::ios::binary | std::ios::ate);

	if (!reader.is_open())
		return false;

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &objPS)))
		return false;

	shaderData.clear();
	reader.close();

	return true;
}

bool ObjectRender::CreateInputLayout(ID3D11Device* device)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA}, 
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA}
	};

	HRESULT hr = device->CreateInputLayout(inputDesc, 3, vShaderByteCode.c_str(), vShaderByteCode.length(), &inpLayout);

	return SUCCEEDED(hr);
}
