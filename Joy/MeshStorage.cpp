#include "MeshStorage.h"

MeshStorage::MeshStorage()
{

}

void MeshStorage::Shutdown()
{
	for (Mesh* mesh : meshes)
	{
		mesh->vertexBuffer->Release();
		mesh->diffuseTextureSRV->Release();
		delete mesh;
	}
}


void MeshStorage::LoadAll()
{
	for (UINT i = 0; i < MeshCount; i++)
	{
		import(i);
	}
}

Mesh* MeshStorage::GetMesh(const std::string& name)
{
	for (UINT i = 0; i < MeshCount; i++)
	{
		if (name == meshNames[i])
			return meshes[i];
	}

	return nullptr;
}

Mesh* MeshStorage::GetMesh(UINT index)
{
	if (index >= meshes.size())
		return nullptr;

	return meshes[index];
}

void MeshStorage::import(UINT index)
{
	std::vector<DirectX::XMFLOAT3> pos;
	std::vector<DirectX::XMFLOAT2> uv;
	std::vector<DirectX::XMFLOAT3> norm;

	std::vector<Vertex> verts;

	std::string fileInfo;
	std::ifstream reader;

	float vtxInfo[3]{};

	int faceP;
	int faceT;
	int faceN;

	bool mtlFound = false;
	std::string mtlInfo = "";

	reader.open(meshPath + meshNames[index]);
	if (!reader.is_open())
		return;

	while (reader >> fileInfo)
	{

		if (fileInfo == "v")
		{
			reader >> vtxInfo[0];
			reader >> vtxInfo[1];
			reader >> vtxInfo[2];
			pos.emplace_back(vtxInfo);
		}

		else if (fileInfo == "vt")
		{
			reader >> vtxInfo[0];
			reader >> vtxInfo[1];
			vtxInfo[1] = 1 - vtxInfo[1];
			uv.emplace_back(vtxInfo);
		}

		else if (fileInfo == "vn")
		{
			reader >> vtxInfo[0];
			reader >> vtxInfo[1];
			reader >> vtxInfo[2];
			norm.emplace_back(vtxInfo);
		}

		else if (fileInfo == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				reader >> faceP;
				reader.ignore(1);
				reader >> faceT;
				reader.ignore(1);
				reader >> faceN;

				verts.emplace_back(pos[faceP - 1], uv[faceT - 1], norm[faceN - 1]);
			}

			reader >> fileInfo;
			if (fileInfo[0] != 'f')
				break;
			else
				reader.putback('f');
		}

		else if (fileInfo == "mtllib")
			reader >> mtlInfo;

		else if (fileInfo == "usemtl")
		{
			if (mtlFound)
				continue;

			if (mtlInfo == "")
				continue;

			std::ifstream mtlReader;
			mtlReader.open(meshPath + mtlInfo); // open .mtl file
			if (!reader.is_open())
				continue;

			reader >> fileInfo; // mtl name

			bool found = false;
			while (mtlReader >> mtlInfo && mtlReader.is_open()) // go through .mtl 
			{
				if (mtlInfo == "newmtl")
				{
					mtlReader >> mtlInfo;
					if (mtlInfo == fileInfo) // look for the mtl
						found = true;
				}

				if (found)
				{
					if (mtlInfo == "map_Kd")
					{
						mtlFound = true;
						mtlReader >> mtlInfo;
						mtlReader.close(); // close .mtl
					}
				}
			}
			mtlReader.close();
		}
	}
	reader.close();

	meshes.emplace_back(new Mesh);
	
	meshes.back()->vertexCount = verts.size();
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(Vertex) * verts.size();
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = verts.data();
	data.SysMemPitch = data.SysMemSlicePitch = 0;
	Backend::GetDevice()->CreateBuffer(&desc, &data, &meshes.back()->vertexBuffer);



	if (!mtlFound)
		return;

	// #include "stb_image.h" gave lnk errors

	mtlInfo = meshPath + mtlInfo;
	int x, y, c;
	unsigned char* imgData = stbi_load(mtlInfo.c_str(), &x, &y, &c, 4);
	if (imgData)
		return;
	
	ID3D11Texture2D* texture{};
	HRESULT hr = Backend::CreateConstSRVTexture2D(&texture, imgData, x, y);
	stbi_image_free(imgData);
	if (FAILED(hr))
		return;

	Backend::GetDevice()->CreateShaderResourceView(texture, nullptr, &meshes.back()->diffuseTextureSRV);
	texture->Release();

}
