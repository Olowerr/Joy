#pragma once
#include "InstancedObject.h"
#include "Character.h"

class DecalShadow
{
public:

	DecalShadow();
	void Shutdown();
	void Clear();

	// Functions for decal to work. Bound to Joy.
	bool InitiateRasterizerState();
	void CreateCharacterDecal(Character* character);
	void UpdateCharacterDecal(Character* character);
	ID3D11Buffer* const* getDecalBuffer();
	bool InitiateDecalDepthBuffer();
	ID3D11ShaderResourceView* const* getDecalSRV();
	void CreateDecalDepthCam(Character* character);
	void UpdateDecalDepthCam(Character* character);
	ID3D11Buffer* const* getDecalCamDCBuff();
	void DrawDecalShadowDepth(std::vector<Object*>& objects, std::vector<InstancedObject>& instancedObjects);

private:

	ID3D11InputLayout* inpLayout;

	DirectX::XMFLOAT4 charPos;

	// Decal buffer with character position, updates constantly (Also depth buffer for the decal)
	ID3D11Buffer* decalDCBuff;
	ID3D11DepthStencilView* decalDSView;
	ID3D11Texture2D* decalDepthTex;
	ID3D11ShaderResourceView* decalSRV;
	ID3D11Buffer* decalCamDCBuff;
	D3D11_VIEWPORT decalViewPort;
	DirectX::XMFLOAT4X4 viewOrtMtrx;
	ID3D11RasterizerState* frontFaceCullingRS;

	// Instanced shaders
	ID3D11VertexShader* decalInstanceVS;

	// Normal shaders
	ID3D11VertexShader* decalVS;
	ID3D11PixelShader* decalPS;

	bool LoadShaders();
	bool CreateInputLayout(const std::string& shaderData);

};