#pragma once
#include "InstancedObject.h"
#include "Character.h"

class DecalShadow
{
public:

	DecalShadow();
	void Shutdown();

	// Functions for decal to work. Bound to Joy.
	bool InitiateRasterizerState();
	void CreateCharacterDecal();
	void UpdateCharacterDecal(DirectX::XMFLOAT3 joyPos);
	bool InitiateDecalDepthBuffer();
	void CreateDecalDepthCam();
	void UpdateDecalDepthCam(DirectX::XMFLOAT3 joyPos);
	void DrawDecalShadowDepth(std::vector<Object*>& objects, std::vector<InstancedObject>& instancedObjects, DirectX::XMFLOAT3 joyPos);

	ID3D11PixelShader*& GetDecalPS();
	ID3D11Buffer*& GetDecalDCBuff();
	ID3D11Buffer*& GetDecalCamDCBuff();
	ID3D11ShaderResourceView*& GetDecalSRV();

private:

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

	// Normal shaders
	ID3D11PixelShader* decalPS;

	bool LoadShaders();

};