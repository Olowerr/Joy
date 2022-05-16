#pragma once
#include "Object.h"
#include "CameraBase.h"

class HObject
{
public:
	static HObject& GetInstance()
	{
		static HObject hObject;
		return hObject;
	}

	void Shutdown()
	{
		colourBuffer->Release();
		aabbVertexBuffer->Release();
		aabbIndexBuffer->Release();

		lineLayout->Release();
		lineVS->Release();

		colourPS->Release();
	}

	void Draw(Object* pObject, Camera* camera, bool highLight, bool lines, int currentBB)
	{
		Backend::UpdateBuffer(colourBuffer, (void*)ObjectColour, 16);

		ID3D11DeviceContext* dc = Backend::GetDeviceContext();
		
		if (highLight)
			dc->ClearDepthStencilView(*Backend::GetStandardDSV(), D3D11_CLEAR_DEPTH, 1.f, 0);

		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dc->IASetInputLayout(storage.posOnlyInputLayout);

		dc->VSSetShader(storage.posOnlyVS, nullptr, 0);
		dc->VSSetConstantBuffers(1, 1, camera->GetMatrixBuffer());

		dc->PSSetShader(colourPS, nullptr, 0);
		dc->PSSetConstantBuffers(0, 1, &colourBuffer);

		dc->OMSetRenderTargets(1, Backend::GetMainRTV(), *Backend::GetStandardDSV());

		if (highLight)
			pObject->DrawGeometry();

		if (lines)
			DrawLines(pObject, currentBB);
	}


private:

	HObject()
		:storage(Backend::GetShaderStorage())
	{
		bool succeeded;

		succeeded = InitPipeline();
		assert(succeeded);

		succeeded = InitBuffers();
		assert(succeeded);
	}

	const UINT LineStride = sizeof(DirectX::XMFLOAT3);
	const UINT Offset = 0;

	const FLOAT ObjectColour[4] = { 1.f, 1.f, 0.f, 0.f };
	const FLOAT LineColour[4] = { 0.f, 1.f, 1.f, 0.f };
	const FLOAT LineColour2[4] = { 1.f, 0.f, 1.f, 0.f };

	ID3D11RasterizerState* wireFrameRS;
	ID3D11Buffer* colourBuffer;
	ID3D11Buffer* aabbVertexBuffer;
	ID3D11Buffer* aabbIndexBuffer;
	UINT numIndices;

	ID3D11InputLayout* lineLayout;
	ID3D11VertexShader* lineVS;

	ID3D11PixelShader* colourPS;

	GraphicsStorage& storage;

	void DrawLines(Object* pObject, int currentBB)
	{

		Backend::UpdateBuffer(colourBuffer, (void*)LineColour, 16);

		ID3D11DeviceContext* dc = Backend::GetDeviceContext();

		dc->IASetVertexBuffers(0, 1, &aabbVertexBuffer, &LineStride, &Offset);
		dc->IASetIndexBuffer(aabbIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		dc->IASetInputLayout(lineLayout);

		dc->VSSetShader(lineVS, nullptr, 0);

		DirectX::XMFLOAT3 corners[8];
		bool changed = false;

		Backend::UpdateBuffer(colourBuffer, (void*)LineColour, 16);
		for (int i = 0; i < pObject->GetNumBboxes(); i++)
		{
			if (i == currentBB)
				continue;

			pObject->GetBoundingBox(i).GetCorners(corners);
			Backend::UpdateBuffer(aabbVertexBuffer, corners, sizeof(corners));
			
			dc->DrawIndexed(numIndices, 0, 0);
		}

		Backend::UpdateBuffer(colourBuffer, (void*)LineColour2, 16);
		pObject->GetBoundingBox(currentBB).GetCorners(corners);
		Backend::UpdateBuffer(aabbVertexBuffer, corners, sizeof(corners));

		dc->DrawIndexed(numIndices, 0, 0);

	}

	bool InitPipeline()
	{
		std::string shaderData;

		ID3D11Device* device = Backend::GetDevice();

		D3D11_INPUT_ELEMENT_DESC inputDesc{};
		inputDesc.SemanticName = "POSITION";
		inputDesc.SemanticIndex = 0;
		inputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputDesc.InputSlot = 0;
		inputDesc.AlignedByteOffset = 0;
		inputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputDesc.InstanceDataStepRate = 0;

		if (!Backend::LoadShader(Backend::ShaderPath + "LineVS.cso", &shaderData))
			return false;
	
		if (FAILED(device->CreateInputLayout(&inputDesc, 1, shaderData.c_str(), shaderData.length(), &lineLayout)))
			return false;

		if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &lineVS)))
			return false;


		if (!Backend::LoadShader(Backend::ShaderPath + "ColourPS.cso", &shaderData))
			return false;

		if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &colourPS)))
			return false;
		
		return true;
	}
	bool InitBuffers()
	{
		ID3D11Device* device = Backend::GetDevice();

		DirectX::XMFLOAT3 corners[8]{};
		DirectX::BoundingBox bb;
		bb.GetCorners(corners);

		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = sizeof(DirectX::XMFLOAT3) * 8;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = corners;
		data.SysMemPitch = data.SysMemSlicePitch = 0;

		if (FAILED(device->CreateBuffer(&desc, &data, &aabbVertexBuffer)))
			return false;
		
		int indices[] = { 0,1, 0,3, 1,2, 1,3, 2,3, 0,4, 0,7, 4,5, 4,7, 5,6, 6,7, 1,5, 2,6, 7,2, 0,5, 3,7, 4,6, 5,2 };
		numIndices = sizeof(indices) / sizeof(int);

		desc.ByteWidth = sizeof(indices);
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		data.pSysMem = indices;

		if (FAILED(device->CreateBuffer(&desc, &data, &aabbIndexBuffer)))
			return false;

		if (FAILED(Backend::CreateDynamicCBuffer(&colourBuffer, nullptr, 16)))
			return false;

		return true;
	}
};