#pragma once
#include "MapSections.h"

class HLight
{
public:
	HLight();
	void Shutdown();
	void ShutdownTools();
	~HLight();

	bool InitiateTools(MapDivider& sections);
	bool GenerateLightMaps(MapDivider& sections);
	bool GenerateLightMapsInstanced(Object** objects, UINT amount, ID3D11ShaderResourceView** lightMaps);

	const UINT ShadowMapXY = 2048; // Can be large since only used during load
	const UINT LightMapXY  = 512; 
	const UINT LightMapCSThreadXY = 16;

private:
	
	/*
		ADD COMPUTESHADER WHICH BLURS ILLEGAL TEXELS ON LIGHTMAP
		ADD COMPUTESHADER WHICH BLURS ILLEGAL TEXELS ON LIGHTMAP
		ADD COMPUTESHADER WHICH BLURS ILLEGAL TEXELS ON LIGHTMAP
		ADD COMPUTESHADER WHICH BLURS ILLEGAL TEXELS ON LIGHTMAP
		ADD COMPUTESHADER WHICH BLURS ILLEGAL TEXELS ON LIGHTMAP
		ADD COMPUTESHADER WHICH BLURS ILLEGAL TEXELS ON LIGHTMAP
		ADD COMPUTESHADER WHICH BLURS ILLEGAL TEXELS ON LIGHTMAP	
	*/

	// General
	D3D11_VIEWPORT lightViewPort;
	GraphicsStorage& storage;

	//  --- Light maps ---
	ID3D11VertexShader* lightVS;
	ID3D11ComputeShader* lightCS;
	ID3D11RasterizerState* noCullingRS;
	ID3D11PixelShader* lightPS;
	ID3D11Buffer* lightDataBuffer;

	// --- Shadow mapping ---
	ID3D11DepthStencilView** shadowMapDSV;
	ID3D11ShaderResourceView** shadowMapSRV;
	UINT amount;

	ID3D11Buffer* lightViewProjectBuffer;
	ID3D11RasterizerState* frontFaceCullingRS;

	void DrawShadowMaps(MapDivider& sections);

	// SUN SUN SUN SUN SUN SUN SUN SUN 
	struct DirectionalLight
	{
		float strength;
		DirectX::XMFLOAT3 direction;
		DirectX::XMFLOAT4X4 viewProject;
	};
	DirectionalLight SUN;


	void FillDescriptions(D3D11_TEXTURE2D_DESC* texDesc, D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc,
		D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

	// --- Initiate Functions ---
	bool InitiateShadowMaps(UINT amount);
	bool InitiateShaders();
	bool InitiateBuffers();
	bool InitiateRasterizerStates();

};
