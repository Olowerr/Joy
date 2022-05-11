#pragma once
#include <d3d11.h>

class GraphicsStorage
{
public:
	GraphicsStorage() = default;

	ID3D11VertexShader* posOnlyVS;
	ID3D11InputLayout* posOnlyInputLayout;

	ID3D11InputLayout* objectInputLayout;
	ID3D11VertexShader* objectVS;
	ID3D11PixelShader* objectPS;

	ID3D11VertexShader* objectInstancedVS;
	ID3D11VertexShader* posOnlyInstancedVS;
	
	ID3D11PixelShader* JoyPS;

	void Shutdown()
	{
		posOnlyVS->Release();
		posOnlyInputLayout->Release();

		objectInputLayout->Release();
		objectVS->Release();
		objectPS->Release();

		objectInstancedVS->Release();
		posOnlyInstancedVS->Release();

		JoyPS->Release();
	}

};