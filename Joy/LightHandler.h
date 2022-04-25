#pragma once
#include "ObjectRender.h"

class HLight
{
public:
	HLight();
	void Shutdown();
	~HLight();

	void CreateLightMaps(Object** objects, UINT amount);

	const UINT ShadowMapWidth = 1024;
	const UINT ShadowMapHeight = 1024;

private:
	ID3D11VertexShader* lightVS;
	// Gs maybe?
	// front-face cull RS
	ID3D11PixelShader* lightPS;

	ID3D11Buffer* lightVSBuffer;
	ID3D11Buffer* lightPSBuffer;

	ID3D11DepthStencilView* shadowMapDSV;
	ID3D11ShaderResourceView* shadowMapSRV;

	struct DirectionalLight
	{
		float strength;
		DirectX::XMFLOAT3 direction;
		DirectX::XMFLOAT4X4 viewProject;
	};
	DirectionalLight SUN;

	void DrawShadowMap(Object** objects, UINT amount);


	bool InitiateShadowMap();
};
