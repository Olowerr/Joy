#include "ObjectRender.h"

ObjectRender::ObjectRender()
	:objVS(nullptr), objPS(nullptr), inpLayout(nullptr), bbRTV(nullptr)
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

}

void ObjectRender::Shutdown()
{
	inpLayout->Release();
	objVS->Release();
	objPS->Release();

	cam->Release();
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


	if (!Backend::LoadShader(Backend::ShaderPath + "ObjInstanceVS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &objInstanceVS)))
		return false;

	if (!Backend::LoadShader(Backend::ShaderPath + "ObjInstancePS.cso", &shaderData))
		return false;

	if (FAILED(Backend::GetDevice()->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &objInstancePS)))
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

void ObjectRender::AddDynamic(Object* obj)
{
	objects.emplace_back(obj);
}

void ObjectRender::AddStatic(Object* obj)
{
	staticObjects.emplace_back(obj);
}

void ObjectRender::DrawAll()
{

	//temp
	ID3D11DeviceContext* dc = Backend::GetDeviceContext();
	dc->IASetInputLayout(inpLayout);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dc->VSSetShader(objVS, nullptr, 0);
	dc->VSSetConstantBuffers(1, 1, &cam);

	dc->PSSetShader(objPS, nullptr, 0);
	dc->OMSetRenderTargets(1, bbRTV, nullptr);


	for (Object* obj : objects)
		obj->Draw();
	

}

bool ObjectRender::FinalizeCurrentStatic()
{
	HRESULT hr;
	using namespace DirectX;
	const UINT numStatic = staticObjects.size();

	XMFLOAT4X4* matrices = new XMFLOAT4X4[numStatic];

	for (UINT i = 0; i < numStatic; i++)
		matrices[i] = staticObjects[i]->GetWorldMatrix();

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(XMFLOAT4X4) * numStatic;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = sizeof(XMFLOAT4X4);
	D3D11_SUBRESOURCE_DATA inData;
	inData.pSysMem = matrices;
	inData.SysMemPitch = inData.SysMemSlicePitch = 0;

	ID3D11Buffer* resource;
	hr = Backend::GetDevice()->CreateBuffer(&desc, &inData, &resource);
	delete matrices;

	if (FAILED(hr))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = sizeof(XMFLOAT4X4);
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = numStatic;

	ID3D11ShaderResourceView* tempSRV;
	hr = Backend::GetDevice()->CreateShaderResourceView(resource, &srvDesc, &tempSRV);
	resource->Release();

	if (FAILED(hr))
		return false;


	instances.emplace_back();

	instances.back().instanceCount = staticObjects.size();
	instances.back().indexCount = staticObjects[0]->GetMesh()->vertexCount;
	instances.back().vertexBuffer = staticObjects[0]->GetMesh()->vertexBuffer;
	//instances.back().indexBuffer = staticObjects[0]->GetMesh()->indexBuffer;
	instances.back().transformSRV = tempSRV;
	//instances.back().lightMapsSRV = get tha lightmaps srv

	staticObjects.clear();

	return SUCCEEDED(hr);

}
