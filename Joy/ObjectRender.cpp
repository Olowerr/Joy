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
	bool succeeded = false;

	succeeded = LoadShaders();
	assert(succeeded);

	SetViewPort();
}

bool ObjectRender::LoadShaders()
{
	std::string shaderData;

	if (!backend.LoadShader(Backend::ShaderPath + "ObjVS.cso", &shaderData))
		return false;

	if (!CreateInputLayout(shaderData))
		return false;

	if (FAILED(backend.GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &objVS)))
		return false;

	if (!backend.LoadShader(Backend::ShaderPath + "ObjPS.cso", &shaderData))
		return false;

	if (FAILED(backend.GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &objPS)))
		return false;

	return true;
}

bool ObjectRender::CreateInputLayout(const std::string& shaderData)
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HRESULT hr = backend.GetDevice()->CreateInputLayout(inputDesc, 3, shaderData.c_str(), shaderData.length(), &inpLayout);

	return SUCCEEDED(hr);
}

void ObjectRender::SetViewPort()
{
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = (float)backend.GetWindowWidth();
	viewPort.Height = (float)backend.GetWindowHeight();
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1;
}
