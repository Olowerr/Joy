#include "MeshStorage.h"

TempMeshStorage::TempMeshStorage()
	:meshes{}
{
	/*JOY::data.StoreAll("../Resources/JoyFiles/big3new.joy");

	ObjectInfo& obj = JOY::data.m_objectInfoVec[0];

	Backend::CreateVertexBuffer(&fbxMesh.vertexBuffer, 
		obj.vertex.data(), obj.vertex.size() * sizeof(JOY::Vertex));

	fbxMesh.indexCount = obj.indices.size();

	D3D11_BUFFER_DESC desc{};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.ByteWidth = sizeof(int) * obj.indices.size();;
	desc.CPUAccessFlags = 0;
	desc.StructureByteStride = 0;
	desc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA inData{};
	inData.pSysMem = obj.indices.data();
	inData.SysMemPitch = inData.SysMemSlicePitch = 0;

	Backend::GetDevice()->CreateBuffer(&desc, &inData, &fbxMesh.indexBuffer);

	std::string path = "../Resources/JoyFiles/";
	path += JOY::data.m_materialInfo.material[0].diffuseTexturePath.string;

	int x, y, c;
	unsigned char* imgData = stbi_load(path.c_str(), &x, &y, &c, 4);
	if (!imgData)
		return;

	ID3D11Texture2D* texture{};
	HRESULT hr = Backend::CreateConstSRVTexture2D(&texture, imgData, x, y);
	stbi_image_free(imgData);
	if (FAILED(hr))
		return;

	Backend::GetDevice()->CreateShaderResourceView(texture, nullptr, &fbxMesh.diffuseTextureSRV);
	texture->Release();*/
}

TempMeshStorage::~TempMeshStorage()
{
}

void TempMeshStorage::LoadAllObj()
{
	for (UINT i = 0; i < MeshCount; i++)
	{
		import(i);
	}
}

void TempMeshStorage::UnloadMeshes()
{
	for (Mesh* mesh : meshes)
	{
		// remove if-statements
		if (mesh->vertexBuffer)
			mesh->vertexBuffer->Release();
		if (mesh->diffuseTextureSRV)
			mesh->diffuseTextureSRV->Release();
		if (mesh->indexBuffer)
			mesh->indexBuffer->Release();

		delete mesh;
	}
	meshes.clear();
}

void TempMeshStorage::LoadMenuObjects()
{
	for (UINT i = 0; i < MenuCount; i++)
		import(tastPath + MenuFiles[i]); // meshPath + MenuFiles[i]
}

void TempMeshStorage::LoadEasyObjects()
{
	for (UINT i = 0; i < EasyCount; i++)
		import(meshPath + EasyFiles[i]);
}

Mesh* TempMeshStorage::GetMesh(const std::string& name)
{
	for (UINT i = 0; i < MeshCount; i++)
	{
		if (name == meshNames[i])
			return meshes[i];
	}

	return nullptr;
}

void TempMeshStorage::UnloadDataBase()
{
	JOY::data.UnloadAll();
}

Mesh* TempMeshStorage::GetMesh(UINT index)
{
	if (index >= MeshCount)
		return nullptr;

	return meshes[index];
}

size_t TempMeshStorage::GetMeshCount() const
{
	return meshes.size();
}

void TempMeshStorage::import(UINT index)
{
	std::vector<DirectX::XMFLOAT3> pos;
	std::vector<DirectX::XMFLOAT2> uv;
	std::vector<DirectX::XMFLOAT3> norm;

	std::vector<JOY::Vertex> verts;

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
			vtxInfo[1] = 1.f - vtxInfo[1];
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

				verts.emplace_back(&pos[faceP - 1].x, &uv[faceT - 1].x, &norm[faceN - 1].x);
			}
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

	float tempXmax = -INFINITY;
	float tempXmin = INFINITY;
	float tempYmax = -INFINITY;
	float tempYmin = INFINITY;
	float tempZmax = -INFINITY;
	float tempZmin = INFINITY;
	for (size_t i = 0; i < pos.size(); i++)
	{
		if (pos[i].x > tempXmax)
			tempXmax = pos[i].x;
		if (pos[i].x < tempXmin)
			tempXmin = pos[i].x;
		if (pos[i].y > tempYmax)
			tempYmax = pos[i].y;
		if (pos[i].y < tempYmin)
			tempYmin = pos[i].y;
		if (pos[i].z > tempZmax)
			tempZmax = pos[i].z;
		if (pos[i].z < tempZmin)
			tempZmin = pos[i].z;
	}
	
	objMeshes[index].bBox.Center.x = (tempXmax + tempXmin) / 2;
	objMeshes[index].bBox.Center.y = (tempYmax + tempYmin) / 2;
	objMeshes[index].bBox.Center.z = (tempZmax + tempZmin) / 2;
	objMeshes[index].bBox.Extents.x = (tempXmax - tempXmin) / 2;
	objMeshes[index].bBox.Extents.y = (tempYmax - tempYmin) / 2;
	objMeshes[index].bBox.Extents.z = (tempZmax - tempZmin)/2;

	if (index == 0)
	{
		objMeshes[index].bBox.Extents.y += 1;
	}

	objMeshes[index].indexCount = verts.size();
	Backend::CreateVertexBuffer(&objMeshes[index].vertexBuffer, verts.data(), sizeof(JOY::Vertex)* verts.size());

	if (!mtlFound)
		return;

	mtlInfo = meshPath + mtlInfo;
	int x, y, c;
	unsigned char* imgData = stbi_load(mtlInfo.c_str(), &x, &y, &c, 4);
	if (!imgData)
		return;
	
	ID3D11Texture2D* texture{};
	HRESULT hr = Backend::CreateConstSRVTexture2D(&texture, imgData, x, y);
	stbi_image_free(imgData);
	if (FAILED(hr))
		return;

	Backend::GetDevice()->CreateShaderResourceView(texture, nullptr, &meshes[index]->diffuseTextureSRV);
	texture->Release();

}

void TempMeshStorage::import(const std::string& filePath)
{
	bool succeeded = false;
	HRESULT hr{};

	size_t sizeBefore = JOY::data.m_objectInfoVec.size();
	
	succeeded = JOY::data.StoreAll(filePath);
	if (!succeeded)
		return; // fix better error handling

	size_t meshesFound = JOY::data.m_objectInfoVec.size() - sizeBefore;
	if (!meshesFound)
		return;

	meshes.reserve(meshesFound);
	for (size_t i = 0; i < meshesFound; i++)
	{
		meshes.emplace_back(new Mesh);

		ObjectInfo& object = JOY::data.m_objectInfoVec.at(sizeBefore + i);

		hr = Backend::CreateVertexBuffer(&meshes.back()->vertexBuffer, object.vertex.data(), sizeof(JOY::Vertex) * object.vertex.size());
		if (FAILED(hr))
			return;

		hr = Backend::CreateIndexBuffer(&meshes.back()->indexBuffer, object.indices.data(), sizeof(int) * object.indices.size());
		if (FAILED(hr))
			return;

		meshes.back()->indexCount = object.indices.size();
		
		Backend::CreateConstSRV(&meshes.back()->diffuseTextureSRV, tastPath + JOY::data.GetMaterial(object)->diffuseTexturePath.string);
		// failed -> will be black

		// bounding box
		{   
			float tempXmax = -INFINITY;
			float tempXmin = INFINITY;
			float tempYmax = -INFINITY;
			float tempYmin = INFINITY;
			float tempZmax = -INFINITY;
			float tempZmin = INFINITY;
			for (JOY::Vertex& vert : object.vertex)
			{

				if (vert.pos[0] > tempXmax)
					tempXmax = vert.pos[0];
				if (vert.pos[0] < tempXmin)
					tempXmin = vert.pos[0];
				if (vert.pos[1] > tempYmax)
					tempYmax = vert.pos[1];
				if (vert.pos[1] < tempYmin)
					tempYmin = vert.pos[1];
				if (vert.pos[2] > tempZmax)
					tempZmax = vert.pos[2];
				if (vert.pos[2] < tempZmin)
					tempZmin = vert.pos[2];
			}

			meshes.back()->bBox.Center.x = (tempXmax + tempXmin) / 2;
			meshes.back()->bBox.Center.y = (tempYmax + tempYmin) / 2;
			meshes.back()->bBox.Center.z = (tempZmax + tempZmin) / 2;

			meshes.back()->bBox.Extents.x = (tempXmax - tempXmin) / 2;
			meshes.back()->bBox.Extents.y = (tempYmax - tempYmin) / 2;
			meshes.back()->bBox.Extents.z = (tempZmax - tempZmin) / 2;
		}
		
	}


}
