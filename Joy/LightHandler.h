#pragma once
#include "ObjectRender.h"

class HLight
{
public:
	HLight(ObjectRender& objRender);
	void Shutdown();
	~HLight();

	void GenerateLightMaps(Object** objects, UINT amount);

	const UINT ShadowMapWidth  = 2048; // Can be large since only used during load
	const UINT ShadowMapHeight = 2048; // Can be large since only used during load
	
	const UINT LightMapWidth  = 512; 
	const UINT LightMapHeight = 512;

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
	ObjectRender& objRender;
	D3D11_VIEWPORT lightViewPort;
	
	//  --- Light maps ---
	ID3D11VertexShader* lightVS;
	ID3D11GeometryShader* lightGS;
	ID3D11RasterizerState* noCullingRS;
	ID3D11PixelShader* lightPS;
	ID3D11Buffer* lightDataBuffer;

	// --- Shadow mapping ---
	ID3D11DepthStencilView* shadowMapDSV;
	ID3D11ShaderResourceView* shadowMapSRV;
	ID3D11Buffer* lightViewProjectBuffer;
	ID3D11RasterizerState* frontFaceCullingRS;
	void DrawShadowMap(Object** objects, UINT amount);

	// SUN SUN SUN SUN SUN SUN SUN SUN 
	struct DirectionalLight
	{
		float strength;
		DirectX::XMFLOAT3 direction;
		DirectX::XMFLOAT4X4 viewProject;
	};
	DirectionalLight SUN;


	// --- Initiate Functions ---
	bool InitiateShadowMap();
	bool InitiateShaders();
	bool InitiateBuffers();
	bool InitiateRasterizerStates();
};
