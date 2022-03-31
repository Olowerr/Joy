#include "ObjectRender.h"

ObjectRender::ObjectRender()
	:backend(Backend::Get()), objVS(nullptr), objPS(nullptr), inpLayout(nullptr), viewPort()
{
}

ObjectRender::~ObjectRender()
{
}

void ObjectRender::initiate()
{
	LoadShaders();
	CreateInputLayout();
	SetViewPort();
}

bool ObjectRender::LoadShaders()
{
	std::string shaderData = "../Shaders/ObjVS.cso";
	backend
	std::ifstream reader;
	reader.open("../Shaders/ObjVS.cso", std::ios::binary | std::ios::ate);

	if (!reader.is_open())
		return false;

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	if (FAILED(backend.GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &objVS)))
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

	if (FAILED(backend.GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &objPS)))
		return false;

	shaderData.clear();
	reader.close();

	return true;
}

bool ObjectRender::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA}, 
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA}
	};

	HRESULT hr = backend.GetDevice()->CreateInputLayout(inputDesc, 3, vShaderByteCode.c_str(), vShaderByteCode.length(), &inpLayout);

	return SUCCEEDED(hr);
}

void ObjectRender::SetViewPort()
{
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = backend.GetWindowWidth();
	viewPort.Height = backend.GetWindowHeight();
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
}
