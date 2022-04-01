#pragma once
#pragma once
#include <fstream>
#include <iostream>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include "Object.h"
#include "Material.h"

struct VertexHolder
{
	float v[3];
	float vn[3];
	float vt[2];

	VertexHolder(DirectX::XMFLOAT3 position,
		DirectX::XMFLOAT3 normal,
		DirectX::XMFLOAT2 uvCoord)
	{
		v[0] = position.x;
		v[1] = position.y;
		v[2] = position.z;
		vn[0] = normal.x;
		vn[1] = normal.y;
		vn[2] = normal.z;
		vt[0] = uvCoord.x;
		vt[1] = uvCoord.y;
	}
};

struct Vertices
{
	std::vector<DirectX::XMFLOAT3> vertex;
	std::vector<DirectX::XMFLOAT2> vertexUV;
	std::vector<DirectX::XMFLOAT3> vertexNormal;
};

struct MaterialValues
{
	DirectX::XMFLOAT3 ka;
	DirectX::XMFLOAT3 kd;
	DirectX::XMFLOAT3 ks;
};

class ObjParser
{
public:

	ObjParser();
	~ObjParser();

	bool LoadObj(std::vector<VertexHolder>& vertsHolder, std::vector<int>& vertices, std::vector<Object>& objectVect);


private:

};