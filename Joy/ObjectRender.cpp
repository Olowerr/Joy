#include "ObjectRender.h"

ObjectRender::ObjectRender()
	:objVS(nullptr), objPS(nullptr), inpLayout(nullptr), viewPort(), bbRTV(nullptr)
{
}

void ObjectRender::Shutdown()
{
	inpLayout->Release();
	objVS->Release();
	objPS->Release();

	cam->Release();
}

void ObjectRender::Initiate()
{
	bool succeeded = false;

	succeeded = LoadShaders();
	assert(succeeded);

	using namespace DirectX;
	XMFLOAT4X4 matri;
	XMMATRIX view = XMMatrixLookAtLH(XMVectorSet(3.f, 3.f, 3.f, 0.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(0.f, 1.f, 0.f, 0.f));
	XMMATRIX proj = XMMatrixPerspectiveFovLH(0.8f, 2.f, 0.1f, 100.f);
	XMStoreFloat4x4(&matri, XMMatrixTranspose(view * proj));

	Backend::CreateConstCBuffer(&cam, &matri, 64);

	bbRTV = Backend::GetBackBufferRTV();

	//temp
	ID3D11DeviceContext* dc = Backend::GetDeviceContext();
	dc->IASetInputLayout(inpLayout);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dc->VSSetShader(objVS, nullptr, 0);
	dc->VSSetConstantBuffers(1, 1, &cam);

	dc->PSSetShader(objPS, nullptr, 0);
	dc->OMSetRenderTargets(1, bbRTV, nullptr);

}

bool ObjectRender::LoadShaders()
{
	std::string shaderData;

	if (!Backend::LoadShader(Backend::ShaderPath + "ObjVS.cso", &shaderData))
		return false;

	if (!CreateInputLayout(shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &objVS)))
		return false;

	if (!Backend::LoadShader(Backend::ShaderPath + "ObjPS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &objPS)))
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

	HRESULT hr = Backend::GetDevice()->CreateInputLayout(inputDesc, 3, shaderData.c_str(), shaderData.length(), &inpLayout);

	return SUCCEEDED(hr);
}

void ObjectRender::Add(Object* obj)
{
	obs.emplace_back(obj);
}

void ObjectRender::DrawAll()
{
	ID3D11DeviceContext* dc = Backend::GetDeviceContext();

	float colour[4] = { 0.2f, 0.2f,0.2f, 0.f };
	dc->ClearRenderTargetView(*bbRTV, colour);

	for (Object* obj : obs)
		obj->Draw();
	

	Backend::GetSwapChain()->Present(0, 0);
}
