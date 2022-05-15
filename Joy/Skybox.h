#pragma once

#include "CameraBase.h"
#include <DirectXMath.h>

class Skybox
{
public:
	Skybox()
		:skySRV(nullptr), dsState(nullptr)
		, vs(nullptr), ps(nullptr), iBuffer(nullptr), idxCount(0)
	{

	}
	void init()
	{
		HRESULT hr;

		ID3D11Device* device = Backend::GetDevice();

		int x, y, c;
		unsigned char* imgData = stbi_load("../Resources/Images/sky.jpg", &x, &y, &c, 4);
		if (!imgData)
			return;

		D3D11_TEXTURE2D_DESC desc{};
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ArraySize = 1;
		desc.MipLevels = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.Height = y;
		desc.Width = x;
		desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data{};
		data.pSysMem = imgData;
		data.SysMemPitch = x * 4;
		data.SysMemSlicePitch = 0;

		ID3D11Texture2D* texCube;
		hr = device->CreateTexture2D(&desc, &data, &texCube);
		if (FAILED(hr))
			return;

		hr = device->CreateShaderResourceView(texCube, nullptr, &skySRV);

		texCube->Release();
		stbi_image_free(imgData);
		if (FAILED(hr))
			return;

		DirectX::XMFLOAT3 pos[4];
		pos[0].x = -1.f; pos[0].y =  1.f; pos[0].z = 1.f;
		pos[1].x =  1.f; pos[1].y =  1.f; pos[1].z = 1.f;
		pos[2].x = -1.f; pos[2].y = -1.f; pos[2].z = 1.f;
		pos[3].x =  1.f; pos[3].y = -1.f; pos[3].z = 1.f;

		DirectX::XMFLOAT2 uv[4];
		uv[0].x = 0.f; uv[0].y = 0.f;
		uv[1].x = 1.f; uv[1].y = 0.f;
		uv[2].x = 0.f; uv[2].y = 1.f;
		uv[3].x = 1.f; uv[3].y = 1.f;

		UINT indices[] = { 0,1,2, 1,3,2 };
		idxCount = 6;


		D3D11_BUFFER_DESC bDesc{};
		bDesc.ByteWidth = sizeof(pos);
		bDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bDesc.CPUAccessFlags = 0;
		bDesc.MiscFlags = 0;
		bDesc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA bData;
		bData.pSysMem = pos;
		bData.SysMemPitch = bData.SysMemSlicePitch = 0;
		
		hr = device->CreateBuffer(&bDesc, &bData, &vBuffer[0]);
		if (FAILED(hr))
			return;

		bDesc.ByteWidth = sizeof(uv);
		bData.pSysMem = uv;
		hr = device->CreateBuffer(&bDesc, &bData, &vBuffer[1]);
		if (FAILED(hr))
			return;

		bDesc.ByteWidth = sizeof(indices);
		bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bData.pSysMem = indices;
		hr = device->CreateBuffer(&bDesc, &bData, &iBuffer);
		if (FAILED(hr))
			return;

		D3D11_DEPTH_STENCIL_DESC dsDesc;
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // Comparison_Equal no bueno
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = 0;
		dsDesc.StencilWriteMask = 0;

		hr = device->CreateDepthStencilState(&dsDesc, &dsState);
		if (FAILED(hr))
			return;

		Backend::CreateDynamicCBuffer(&camPosBuffer, nullptr, 16);

		initPipeline();
	}
	void Shutdown()
	{
		skySRV->Release();
		vBuffer[0]->Release();
		vBuffer[1]->Release();
		iBuffer->Release();
		camPosBuffer->Release();
		il->Release();

		vs->Release();
		ps->Release();
		dsState->Release();
	}
	~Skybox()
	{

	}

	void Draw(Camera* camera)
	{

		static UINT stride[2] = { sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT2) }, offset[2] = {0,0};

		ID3D11DeviceContext* dc = Backend::GetDeviceContext();


		static ID3D11Buffer* temp[2] = {};

		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, camera->GetPosition());
		Backend::UpdateBuffer(camPosBuffer, &pos, 12);

		temp[0] = *camera->GetMatrixBuffer();
		temp[1] = camPosBuffer;

		dc->IASetInputLayout(il);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dc->IASetVertexBuffers(0, 2, vBuffer, stride, offset);
		dc->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);

		dc->VSSetShader(vs, nullptr, 0);
		dc->VSSetConstantBuffers(0, 2, temp);

		dc->PSSetShader(ps, nullptr, 0);
		dc->PSSetShaderResources(0, 1, &skySRV);

		dc->OMSetDepthStencilState(dsState, 0);

		dc->OMSetRenderTargets(1, Backend::GetMainRTV(), *Backend::GetStandardDSV());

		dc->DrawIndexed(idxCount, 0, 0);

		dc->OMSetDepthStencilState(nullptr, 0);
	}

	ID3D11ShaderResourceView* GetSkyboxSRV()
	{
		return skySRV;
	}

private:

	void initPipeline()
	{
		ID3D11Device* device = Backend::GetDevice();

		std::string shaderData;
		Backend::LoadShader(Backend::ShaderPath + "SkyBoxVS.cso", &shaderData);

		if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &vs)))
			return;

		D3D11_INPUT_ELEMENT_DESC inputDesc[2] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		device->CreateInputLayout(inputDesc, 2, shaderData.c_str(), shaderData.size(), &il);

		Backend::LoadShader(Backend::ShaderPath + "SkyBoxPS.cso", &shaderData);

		if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &ps)))
			return;
	}

	ID3D11ShaderResourceView* skySRV;
	ID3D11Buffer* vBuffer[2];
	ID3D11Buffer* iBuffer;
	UINT idxCount;
	ID3D11InputLayout* il;

	ID3D11Buffer* camPosBuffer;
	ID3D11DepthStencilState* dsState;
	ID3D11VertexShader* vs;
	ID3D11PixelShader* ps;
};