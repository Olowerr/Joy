#include "Writer.h"

std::vector<Vertex> Writer::vertices;
std::vector<VerticeIdx> Writer::polygons;
std::vector<Material> Writer::materials;
//std::vector<Texture> Writer::textures;
std::vector<int> Writer::matIdx;
std::vector<Camera> Writer::cameras;
std::vector<Light> Writer::lights;
std::vector<NamedKeyframes> Writer::keyframes;
std::vector<Group> Writer::groups;
AnimationData Writer::aniData;
bool Writer::isGroup = false;
int Writer::bindPoseIdx = 0;
int Writer::posIdx = 0;
int Writer::dirIdx = 0;
int Writer::keyframeCounter = 0;
int Writer::instCounter = 0;
bool Writer::isKeyframe = false;
bool Writer::hasBi = false;

std::vector<MeshData> Writer::meshes;

#define CCP (const char*)

void Writer::WriteFile(cStringR filePath)
{
	
	std::ofstream writer;
	writer.open("../../Resources/JOYFiles/" + filePath, std::ios::binary | std::ios::trunc);
	if (!writer.is_open())
		return;

	int bytesIncoming = 0; // for testing

	GeneralHeader genHeader;
	genHeader.totalByteSize = sizeof(GeneralHeader) - sizeof(Header);
	genHeader.animationData = aniData;
	genHeader.numMeshes	= (int)meshes.size();
	genHeader.numMaterials = (int)materials.size();
	//genHeader.numTextures =	(int)textures.size();
	genHeader.numCameras = (int)cameras.size();
	genHeader.numLights	= (int)lights.size();
	genHeader.numGroups = (int)groups.size();

	printf("\nData Found:\n");
	printf("FPS: %f    Start frame %d    End frame %d\n", aniData.frameRate, aniData.startFrame, aniData.endFrame);

	printf("Number of meshes: %d\n    ", genHeader.numMeshes);
	for (MeshData& mesh : meshes)
		printf("%s %s  |  ", mesh.name.string, mesh.weightVtx.size() ? ": Skinned" : "");

	printf("\nNumber of materials: %d\n    ", genHeader.numMaterials);
	for (Material& mat : materials)
		printf("%s  |  ", mat.matName.string);

	//printf("\nNumber of textures: %d", genHeader.numTextures);
	printf("\nNumber of cameras: %d", genHeader.numCameras);
	printf("\nNumber of lights: %d", genHeader.numLights);
	printf("\nNumber of groups: %d", genHeader.numGroups);

	printf("\n\nWriting %s...\n", filePath.c_str());

	writer.write(CCP & genHeader, sizeof(GeneralHeader));

	int skeletonId = 0, propertyId = 0, morphId = 0;
	ObjectHeader obj;
	for (MeshData& mesh : meshes)
	{
		bytesIncoming = 0;

		mesh.skeletonId = mesh.weightVtx.size() ? skeletonId++ : -1;
		mesh.morphId = mesh.deformers.size() ? morphId++ : -1;
		mesh.propertyId = mesh.props.size() ? propertyId++ : -1;
		MakeUnique(mesh);
		
		obj.meshName = mesh.name;
		obj.parentName = mesh.parent;
		obj.materialName = mesh.mtlName;
		obj.numIdx = (int)mesh.idx.size();
		obj.numVerts = (int)mesh.verts.size();
		obj.hasBi = (int)mesh.biNorms.size();
		obj.skeletonIdx = mesh.skeletonId;
		obj.morphIdx = mesh.morphId;
		obj.propertyId = mesh.propertyId;
		obj.numChildren = (int)mesh.childrenNames.size();
		obj.totalByteSize = int(obj.numVerts * sizeof(Vertex)) + int(mesh.biNorms.size() * sizeof(BiNormals))
			+ int(mesh.weightVtx.size() * sizeof(WeightVTX)) + int(mesh.idx.size() * sizeof(int))
			+ obj.numChildren * sizeof(JoyString) + int(sizeof(ObjectHeader) - sizeof(Header));

		writer.write(CCP & obj, sizeof(ObjectHeader));
		bytesIncoming += sizeof(ObjectHeader) - sizeof(Header); // don't care about Header size since already read
		
		for (int i = 0; i < obj.numChildren; i++)
			writer.write(CCP & mesh.childrenNames.at(i), sizeof(JoyString));

		writer.write(CCP mesh.verts.data(), sizeof(Vertex) * obj.numVerts);
		bytesIncoming += sizeof(Vertex) * obj.numVerts;

		if (obj.hasBi)
		{
			writer.write(CCP mesh.biNorms.data(), sizeof(BiNormals) * obj.numVerts);
			bytesIncoming += sizeof(BiNormals) * obj.numVerts;
		}

		if (obj.skeletonIdx != -1)
		{
			writer.write(CCP mesh.weightVtx.data(), sizeof(WeightVTX) * obj.numVerts);
			bytesIncoming += sizeof(WeightVTX) * obj.numVerts;
		}

		writer.write(CCP mesh.idx.data(), sizeof(int) * obj.numIdx);
		bytesIncoming += sizeof(int) * obj.numIdx;

	}

	if (materials.size() > 0)
	{
		bytesIncoming = 0;
		MaterialHeader mat;
		mat.totalByteSize = int(materials.size() * sizeof(Material));
		writer.write(CCP & mat, sizeof(MaterialHeader));
		bytesIncoming += sizeof(MaterialHeader) - sizeof(Header); // don't care about Header size since already read
		for (Material& mats : materials)
		{
			writer.write(CCP & mats, sizeof(mats));
			bytesIncoming += sizeof(mats);
		}
	}

	//if (textures.size() > 0)
	//{
	//	bytesIncoming = 0;
	//	TextureHeader texture;
	//	texture.totalByteSize = int(textures.size() * sizeof(Texture));

	//	writer.write(CCP & texture, sizeof(TextureHeader));
	//	bytesIncoming += sizeof(TextureHeader) - sizeof(Header); // don't care about Header size since already read

	//	for (Texture& texts : textures)
	//	{
	//		writer.write(CCP & texts, sizeof(texts));
	//		bytesIncoming += sizeof(texts);
	//	}
	//}


	int framesSize;
	int idxCounter = 0;
	
	SkeletonHeader skeleton;
	for (int i = 0; i < meshes.size(); i++)
	{
		bytesIncoming = 0;
		framesSize = 0;
		for (Joint& joint : meshes[i].joints)
			framesSize += joint.numFrames * sizeof(KeyFrame);

		if (meshes[i].joints.size() > 0)
		{
			skeleton.id = meshes[i].skeletonId;
			skeleton.numJoints = (int)meshes[i].joints.size();
			skeleton.totalByteSize = int(meshes[i].joints.size() * sizeof(Joint)) + framesSize + int(sizeof(SkeletonHeader) - sizeof(Header));
			writer.write(CCP & skeleton, sizeof(SkeletonHeader));
			bytesIncoming += sizeof(SkeletonHeader) - sizeof(Header);
			for (Joint& pose : meshes[i].joints)
			{
				writer.write(CCP & pose, sizeof(pose));
				bytesIncoming += sizeof(pose);
				if (pose.numFrames > 0)
				{
					for (int k = 0; k < pose.numFrames; k++)
					{
						writer.write(CCP & keyframes[idxCounter].frame, sizeof(keyframes[idxCounter].frame));
						idxCounter++;
						bytesIncoming += sizeof(keyframes[idxCounter].frame);
					}
				}
			}
		}
	}

	MorphHeader morphHeader;
	MorphTargetInfo morphInfo;
	for (MeshData& mesh : meshes)
	{
		if (!mesh.deformers.size())
			continue;
		
		bytesIncoming = 0;
		int deformerSize = 0;
		for (MeshDeformer* deformer : mesh.deformers)
		{
			deformerSize += sizeof(MorphFrame) * (int)deformer->frames.size();
			deformerSize += sizeof(MorphVTX) * (int)deformer->vertices.size();
		}


		morphHeader.id = mesh.morphId;
		morphHeader.numDeformers = (int)mesh.deformers.size();
		morphHeader.totalByteSize = int(sizeof(MorphHeader) - sizeof(Header)) + sizeof(MorphTargetInfo) * (int)mesh.deformers.size()
			+ deformerSize;
		
		// don't care about Header size since already read

		writer.write(CCP& morphHeader, sizeof(MorphHeader));
		bytesIncoming += sizeof(MorphHeader) - sizeof(Header);

		for (MeshDeformer* deformer : mesh.deformers)
		{
			morphInfo.name = deformer->shapeName;
			morphInfo.numFrames = (int)deformer->frames.size();

			writer.write(CCP& morphInfo, sizeof(MorphTargetInfo));
			bytesIncoming += sizeof(MorphTargetInfo);

			writer.write(CCP deformer->vertices.data(), sizeof(MorphVTX) * deformer->vertices.size());
			bytesIncoming += sizeof(MorphVTX) * (int)deformer->vertices.size();
			
			writer.write(CCP deformer->frames.data(), sizeof(MorphFrame) * deformer->frames.size());
			bytesIncoming += sizeof(MorphFrame) * (int)deformer->frames.size();

		}
	}

	PropertiesHeader praprap;
	for (MeshData& mesh : meshes)
	{
		bytesIncoming = 0;
		if (!mesh.props.size())
			continue;

		praprap.totalByteSize = sizeof(PropertiesHeader) - sizeof(Header);
		for (PropertyBase* prop : mesh.props)
			praprap.totalByteSize += (int)_msize(prop);
		
		praprap.numProperties = (int)mesh.props.size();
		praprap.propertyId = mesh.propertyId;
		
		writer.write(CCP& praprap, sizeof(PropertiesHeader));
		bytesIncoming += sizeof(PropertiesHeader) - sizeof(Header);

		for (PropertyBase* prop : mesh.props)
		{
			writer.write(CCP prop, _msize(prop));
			bytesIncoming += (int)_msize(prop);
		}
	}

	if (cameras.size() > 0)
	{
		bytesIncoming = 0;
		CameraHeader camera;
		camera.totalByteSize = int(cameras.size() * sizeof(Camera));

		writer.write(CCP & camera, sizeof(CameraHeader));
		bytesIncoming += sizeof(CameraHeader) - sizeof(Header); // don't care about Header size since already read

		for (Camera& cams : cameras)
		{
			writer.write(CCP & cams, sizeof(cams));
			bytesIncoming += sizeof(cams);
		}
	}

	if (lights.size() > 0)
	{
		bytesIncoming = 0;
		LightHeader light;
		light.totalByteSize = int(lights.size() * sizeof(Light));

		writer.write(CCP & light, sizeof(LightHeader));
		bytesIncoming += sizeof(LightHeader) - sizeof(Header); // don't care about Header size since already read

		for (Light& lghts : lights)
		{
			writer.write(CCP & lghts, sizeof(lghts));
			bytesIncoming += sizeof(lghts);
		}
	}

	if (groups.size() > 0)
	{
		bytesIncoming = 0;
		GroupHeader group;
		group.totalByteSize = int(groups.size() * sizeof(Group));

		writer.write(CCP & group, sizeof(GroupHeader));
		bytesIncoming += sizeof(GroupHeader) - sizeof(Header); // don't care about Header size since already read

		for (Group& grp : groups)
		{
			writer.write(CCP & grp, sizeof(grp));
			bytesIncoming += sizeof(grp);
		}
	}

	writer.close();

	//ReadFile(filePath);

	/*
	props.emplace_back(new Prop<float>(Property::TYPE::FLOAT, 1.f));
	props.emplace_back(new Prop<bool>		 (Property::TYPE::BOOL,		true)	 );
	props.emplace_back(new Prop<std::string> (Property::TYPE::STRING,	"yello") );

	std::ofstream writer2;
	writer2.open("Thomas.joy", std::ios::binary | std::ios::trunc);
	if (!writer2.is_open())
		return;
	writer2.write((const char*)props[0], sizeof(props[0]));
	writer2.close();


	std::ifstream reader2;
	reader2.open("Thomas.joy", std::ios::binary);
	if (!reader2.is_open())
		return;

	Property prap;
	reader2.read((char*)&prap, sizeof(Property));
	
	// if prap.type == float
	Prop<float> me(Property::TYPE::FLOAT);
	char* ptr = (char*)&me;
	ptr += sizeof(Property);

	reader2.read(ptr, sizeof(Prop<float>) - sizeof(Property));
	reader2.close();
	*/

	/*std::ofstream wri;
	wri.open("Thomas.joy", std::ios::binary | std::ios::trunc);
	if (!wri.is_open())
		return;


	FbxString test("yealllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllllo");
	Prop<JoyStr> stringProp(Property::TYPE::STRING, test);

	Property* que = new Prop<Float3>(Property::TYPE::VECTOR, Float3(4.f, 2.f, 3.f));

	int q = (int)_msize(que);
	wri.write((const char*)que, _msize(que));
	//wri.write((const char*)&stringProp, sizeof(stringProp));
	wri.close();

	std::ifstream rei;
	rei.open("Thomas.joy", std::ios::binary);
	if (!rei.is_open())
		return;

	Property type;
	rei.read((char*)&type, sizeof(Property));

	// if type == 

	Prop<Float3> boi(Property::TYPE::VECTOR);
	//Prop<JoyStr> boi(Property::TYPE::STRING);
	char* ptr2 = (char*)&boi + sizeof(Property);

	rei.read(ptr2, sizeof(boi) - sizeof(Property));
	rei.close();
	*/
	
	/*
	std::vector<int> indices;
	std::vector<Vertex> uniqueVerts;
	uniqueVerts.reserve(vertices.size());

	int index = -1;
	for (UINT i = 0; i < vertices.size(); i++)
	{
		index = -1;

		for (UINT k = 0; k < uniqueVerts.size(); k++)
		{
			if (vertices[i] == uniqueVerts[k])
			{
				index = k;
				k = (int)uniqueVerts.size();
			}
		}

		if (index == -1)
		{
			uniqueVerts.emplace_back(vertices[i]);
			indices.emplace_back((int)uniqueVerts.size() - 1);
		}
		else
			indices.emplace_back(index);
	}

	for (int i = 0; i < matIdx.size(); i++)
	{
		polygons.emplace_back(indices[i], indices[i + 1], indices[i + 2]);
	}
	sortMaterial();

	std::ofstream writer;
	writer.open("Thomas.joy", std::ios::binary | std::ios::trunc);
	if (!writer.is_open())
		return;
	
	int vertCount = (int)uniqueVerts.size();
	int idxCount = (int)indices.size();
	
	writer.write((const char*)&vertCount, 4);
	writer.write((const char*)&idxCount, 4);
	writer.write((const char*)&hasBi, 1);

	for (size_t i = 0; i < vertCount; i++)
	{
		if (hasBi)
			writer.write((const char*)&uniqueVerts[i], sizeof(Vertex));
		else
			writer.write((const char*)&uniqueVerts[i], sizeof(Vertex));
	}

	for (VerticeIdx& idx : polygons)
		writer.write((const char*)&idx, 12);
	

	writer.close();


	vertices.clear();


	std::ifstream reader;
	reader.open("Thomas.joy", std::ios::binary);
	if (!reader.is_open())
		return;

	int readVertCount = 0;
	int readIdxCount = 0;
	bool readHasBiNormal = false;

	reader.read((char*)&readVertCount, 4);
	reader.read((char*)&readIdxCount, 4);
	reader.read((char*)&readHasBiNormal, 1);

	std::vector<int> idxs;
	for (size_t i = 0; i < readVertCount; i++)
	{
		vertices.emplace_back();
		if (readHasBiNormal)
			reader.read((char*)&vertices[i], sizeof(Vertex));
		else
			reader.read((char*)&vertices[i], sizeof(Vertex));
	}
	for (size_t i = 0; i < readIdxCount; i++)
	{
		idxs.emplace_back();
		reader.read((char*)&idxs[i], 4);
	}

	reader.close();
	*/
}

void Writer::ReadFile(cStringR filePath)
{
	std::ifstream reader;
	reader.open(filePath, std::ios::binary);
	if (!reader.is_open())
		return;

	Header header;

	GeneralHeader genHeader;
	reader.read((char*) & genHeader, sizeof(GeneralHeader));

	for (size_t k = 0; k < meshes.size(); k++)
	{
		reader.read((char*)&header, sizeof(header));
		reader.ignore(header.totalByteSize);
		continue;

		if (header.type == BaseHeader::Object)
		{
			Vertex tVert;
			BiNormals tBi;
			WeightVTX tWeight;
			int tInt;

			ObjectHeader obj;
			char* ptr = (char*)&obj + sizeof(Header);
			reader.read(ptr, sizeof(ObjectHeader) - sizeof(Header));

			JoyString childName;
			for (int i = 0; i < obj.numChildren; i++)
			{
				reader.read((char*)&childName, sizeof(JoyString));
				int q = 0;
			}

			for (int i = 0; i < obj.numVerts; i++)
				reader.read((char*)&tVert, sizeof(Vertex));

			if (obj.hasBi)
			{
				for (int i = 0; i < obj.numVerts; i++)
					reader.read((char*)&tBi, sizeof(BiNormals));
			}
			if (obj.skeletonIdx != -1)
			{
				for (int i = 0; i < obj.numVerts; i++)
					reader.read((char*)&tWeight, sizeof(WeightVTX));
			}
			for (int i = 0; i < obj.numIdx; i++)
				reader.read((char*)&tInt, 4);

			int q = 0;
		}
	}
	

	if (materials.size())
	{
		reader.read((char*)&header, sizeof(header));
		if (header.type == BaseHeader::Material)
		{
			MaterialHeader mHeader;
			char* ptr = (char*) & mHeader + sizeof(Header);

			// Reading 0 bytes, the header doesn't contain anything extra
			reader.read(ptr, sizeof(MaterialHeader) - sizeof(Header)); 
			
			for (int i = 0; i < materials.size(); i++)
			{
				Material mats;
				reader.read((char*)&mats, sizeof(mats));
				int q = 0;
			}
		}
	}

	//if (textures.size())
	//{
	//	reader.read((char*)&header, sizeof(header));
	//	if (header.type == BaseHeader::Texture)
	//	{
	//		TextureHeader tHeader;
	//		char* ptr = (char*)&tHeader + sizeof(Header);

	//		// Reading 0 bytes, the header doesn't contain anything extra
	//		reader.read(ptr, sizeof(TextureHeader) - sizeof(Header));
	//		
	//		for (int i = 0; i < textures.size(); i++)
	//		{
	//			Texture texts;
	//			reader.read((char*)&texts, sizeof(texts));
	//		}
	//	}
	//}

	SkeletonHeader skeleton;
	for (int i = 0; i < meshes.size(); i++)
	{
		if (meshes[i].joints.size() > 0)
		{
			reader.read((char*)&header, sizeof(header));
			if (header.type == BaseHeader::Skeleton)
			{
				char* ptr = (char*)&skeleton + sizeof(Header);
				reader.read(ptr, sizeof(SkeletonHeader) - sizeof(Header));
				for (int j = 0; j < skeleton.numJoints; j++)
				{
					Joint pose;
					reader.read((char*)&pose, sizeof(pose));

					if (pose.numFrames > 0)
					{
						for (int f = 0; f < pose.numFrames; f++)
						{
							KeyFrame keys;
							reader.read((char*)&keys, sizeof(keys));
						}
					}
				}
			}
		}
	}

	for (MeshData& mesh : meshes)
	{
		if (!mesh.deformers.size())
			continue;

		reader.read((char*)&header, sizeof(Header));
		if (header.type != BaseHeader::Morph)
			continue;

		MorphHeader morphHeader;
		char* ptr = (char*)&morphHeader + sizeof(Header);

		reader.read(ptr, sizeof(MorphHeader) - sizeof(Header));
		
		MorphTargetInfo morphInfo;
		for (int i = 0; i < morphHeader.numDeformers; i++)
		{
			reader.read((char*)&morphInfo, sizeof(MorphTargetInfo));

			MorphVTX morphVtx;
			for (int k = 0; k < mesh.deformers[i]->vertices.size(); k++)
				reader.read((char*)&morphVtx, sizeof(MorphVTX));

			MorphFrame morphFrame;
			for (int k = 0; k < morphInfo.numFrames; k++)
				reader.read((char*)&morphFrame, sizeof(MorphFrame));
		}
	}

	std::vector<PropertyBase*> prap;
	prap.reserve(7);
	for (MeshData& mesh : meshes)
	{
		reader.read((char*)&header, sizeof(header));
		if (header.type != BaseHeader::Property)
			continue;

		PropertiesHeader proHeader;
		char* ptr = (char*)& proHeader + sizeof(Header);
		reader.read(ptr, sizeof(PropertiesHeader) - sizeof(Header));

		Property<Float3>* float3er;
		Property<float>* floater;
		Property<int>* inter;
		Property<bool>* booler;
		Property<JoyString>* strer;
		Property<int>* enumer;

		for (int i = 0; i < proHeader.numProperties; i++)
		{
			PropertyType type;
			reader.read((char*)&type, sizeof(PropertyType));

			switch (type)
			{
			default:
				break;
			case PropertyType::VECTOR:
				prap.emplace_back(new Property<Float3>(PropertyType::VECTOR));
				reader.read((char*)prap.back() + 4, sizeof(Property<Float3>) - sizeof(PropertyType));
				float3er = (Property<Float3>*)prap.back();
				break;

			case PropertyType::FLOAT:
				prap.emplace_back(new Property<float>(PropertyType::FLOAT));
				reader.read((char*)prap.back() + 4, sizeof(Property<float>) - sizeof(PropertyType));
				floater = (Property<float>*)prap.back();
				break;

			case PropertyType::INT:
				prap.emplace_back(new Property<int>(PropertyType::INT));
				reader.read((char*)prap.back() + 4, sizeof(Property<int>) - sizeof(PropertyType));
				inter = (Property<int>*)prap.back();
				break;

			case PropertyType::BOOL:
				prap.emplace_back(new Property<bool>(PropertyType::BOOL));
				reader.read((char*)prap.back() + 4, sizeof(Property<bool>) - sizeof(PropertyType));
				booler = (Property<bool>*)prap.back();
				break;

			case PropertyType::STRING:
				prap.emplace_back(new Property<JoyString>(PropertyType::STRING));
				reader.read((char*)prap.back() + 4, sizeof(Property<JoyString>) - sizeof(PropertyType));
				strer = (Property<JoyString>*)prap.back();
				break;

			case PropertyType::ENUM:
				prap.emplace_back(new Property<int>(PropertyType::ENUM));
				reader.read((char*)prap.back() + 4, sizeof(Property<int>) - sizeof(PropertyType));
				enumer = (Property<int>*)prap.back();
				break;
			}
		}

	}


	if (cameras.size() > 0)
	{
		reader.read((char*)&header, sizeof(header));
		Camera cams;

		if (header.type == BaseHeader::Camera)
		{
			CameraHeader myHeader;
			char* ptr = (char*)&myHeader + sizeof(Header);

			// Reading 0 bytes, the header doesn't contain anything extra
			reader.read(ptr, sizeof(CameraHeader) - sizeof(Header));

			for (int i = 0; i < cameras.size(); i++)
			{
				reader.read((char*)&cams, sizeof(cams));
			}
		}
	}

	if (lights.size() > 0)
	{
		reader.read((char*)&header, sizeof(header));
		if (header.type == BaseHeader::Light)
		{
			LightHeader myHeader;
			char* ptr = (char*)&myHeader + sizeof(Header);

			// Reading 0 bytes, the header doesn't contain anything extra
			reader.read(ptr, sizeof(LightHeader) - sizeof(Header));
			
			for (int i = 0; i < lights.size(); i++)
			{
				Light lghts;
				reader.read((char*)&lghts, sizeof(lghts));
				int q = 0;
			}
		}
	}

	if (groups.size() > 0)
	{
		reader.read((char*)&header, sizeof(header));
		if (header.type == BaseHeader::Group)
		{
			GroupHeader myHeader;
			char* ptr = (char*)&myHeader + sizeof(Header);

			// Reading 0 bytes, the header doesn't contain anything extra
			reader.read(ptr, sizeof(GroupHeader) - sizeof(Header));
			
			for (int i = 0; i < groups.size(); i++)
			{
				Group grp;
				reader.read((char*)&grp, sizeof(grp));
			}
		}
	}

	reader.close();
}

void Writer::MakeUnique(MeshData& mesh)
{
	std::vector<Vertex> uniqueVerts;
	std::vector<BiNormals> uniqueBi;
	std::vector<WeightVTX> uniqueWVTX;
	std::vector<MorphVTX> uniqueMVTX;

	uniqueVerts.reserve(mesh.verts.size());
	if (mesh.biNorms.size())
		uniqueBi.reserve(mesh.verts.size());
	if (mesh.weightVtx.size())
		uniqueWVTX.reserve(mesh.verts.size());
	if (mesh.weightVtx.size())
		uniqueMVTX.reserve(mesh.verts.size());

	int index = -1;
	for (UINT i = 0; i < mesh.verts.size(); i++)
	{
		index = -1;

		for (UINT k = 0; k < uniqueVerts.size(); k++)
		{
			if (mesh.biNorms.size() && mesh.weightVtx.size())
			{
				if (mesh.verts[i] == uniqueVerts[k] && mesh.biNorms[i] == uniqueBi[k] && mesh.weightVtx[i] == uniqueWVTX[k])
				{
					index = k;
					k = (int)uniqueVerts.size();
				}
			}
			else if (mesh.biNorms.size() && !mesh.weightVtx.size())
			{
				if (mesh.verts[i] == uniqueVerts[k] && mesh.biNorms[i] == uniqueBi[k])
				{
					index = k;
					k = (int)uniqueVerts.size();
				}
			}
			else if (!mesh.biNorms.size() && mesh.weightVtx.size())
			{
				if (mesh.verts[i] == uniqueVerts[k] && mesh.weightVtx[i] == uniqueWVTX[k])
				{
					index = k;
					k = (int)uniqueVerts.size();
				}
			}
			else
			{
				if (mesh.verts[i] == uniqueVerts[k])
				{
					index = k;
					k = (int)uniqueVerts.size();
				}
			}
		}

		if (index == -1)
		{
			uniqueVerts.emplace_back(mesh.verts[i]);
			if (mesh.biNorms.size())
				uniqueBi.emplace_back(mesh.biNorms[i]);
			if (mesh.weightVtx.size())
				uniqueWVTX.emplace_back(mesh.weightVtx[i]);
			//if (mesh.morphFrames.size())
			//	uniqueMVTX.emplace_back(mesh.morphVtx[i]);

			mesh.idx.emplace_back((int)uniqueVerts.size() - 1);
		}
		else
			mesh.idx.emplace_back(index);
	}
	mesh.verts = uniqueVerts;
	mesh.biNorms = uniqueBi;
	mesh.weightVtx = uniqueWVTX;
	//mesh.morphVtx = uniqueMVTX;
}

void Writer::AddProp(cStringR meshName, FbxProperty& prop)
{
	std::string name(prop.GetNameAsCStr());
	if (name == "currentUVSet")
		return;

	MeshData* mesh = FindMesh(meshName);
	if (!mesh)
		return;

	switch (prop.GetPropertyDataType().GetType())
	{
	case eFbxDouble3:
	{
		FbxDouble3 doubles = prop.Get<FbxDouble3>();
		Float3 values = { (float)doubles.mData[0], (float)doubles.mData[1], (float)doubles.mData[2] };
		mesh->props.emplace_back(new Property<Float3>(PropertyType::VECTOR, values));
		return;
	}
	case eFbxDouble:
		mesh->props.emplace_back(new Property<float>(PropertyType::FLOAT, prop.Get<FbxFloat>()));
		return;

	case eFbxInt:
		mesh->props.emplace_back(new Property<int>(PropertyType::INT, prop.Get<FbxInt>()));
		return;

	case eFbxBool:
		mesh->props.emplace_back(new Property<bool>(PropertyType::BOOL, prop.Get<FbxBool>()));
		return;

	case eFbxString:
		mesh->props.emplace_back(new Property<JoyString>(PropertyType::STRING, prop.Get<FbxString>().Buffer()));
		return;

	case eFbxEnum:
		mesh->props.emplace_back(new Property<int>(PropertyType::ENUM, prop.Get<FbxInt>()));
		return;
	}

}

void Writer::CreateWeightVtx(FbxNode* pNode)
{
	FbxMesh* fbxMesh = pNode->GetMesh();
	if (!fbxMesh)
		return;

	MeshData* mesh = FindMesh(pNode->GetName());
	if (!mesh)
		return;

	int lPolygonCount = fbxMesh->GetPolygonCount();
	int lPolygonSize = 0;

	for (int i = 0; i < lPolygonCount; i++)
	{
		lPolygonSize = fbxMesh->GetPolygonSize(i);
		for (int k = 0; k < lPolygonSize; k++)
		{
			int lControlPointIndex = fbxMesh->GetPolygonVertex(i, k);

			mesh->vtxCPIdx.emplace_back(lControlPointIndex);	// yeaBOI
			mesh->weightVtx.emplace_back();					   // yeaBOI
			mesh->weightVtxCounter.push_back(0);			  // yeaBOI
		}
	}
}

void Writer::AddJoint(cStringR meshName, cStringR name)
{
	MeshData* mesh = FindMesh(meshName);
	if (mesh)
		mesh->jointNames.emplace_back(name);
}

void Writer::SetWeight(cStringR meshName, int boneIdx, int cpIdxC, int* pCpIdx, double* pWeights)
{
	MeshData* mesh = FindMesh(meshName);
	if (!mesh)
		return;

	for (int i = 0; i < cpIdxC; i++)
	{
		for (int k = 0; k < mesh->vtxCPIdx.size(); k++)
		{
			if (mesh->vtxCPIdx[k] == pCpIdx[i])
			{
				if (mesh->weightVtxCounter[k] >= 4)
					continue;

				mesh->weightVtx[k].weight[mesh->weightVtxCounter[k]] = (float)pWeights[i];
				mesh->weightVtx[k].boneIdx[mesh->weightVtxCounter[k]] = boneIdx;
				mesh->weightVtxCounter[k]++;
			}
		}
	}
}

void Writer::NormalizeWeights(cStringR meshName)
{
	MeshData* mesh = FindMesh(meshName);
	if (!mesh)
		return;

	float sum, sumInverse;
	for (WeightVTX& vtx : mesh->weightVtx)
	{
		sum = vtx.weight[0] + vtx.weight[1] + vtx.weight[2] + vtx.weight[3];
		if (sum != 1.f)
		{
			sumInverse = 1.0f / sum;
			vtx.weight[0] *= sumInverse;
			vtx.weight[1] *= sumInverse; 
			vtx.weight[2] *= sumInverse;
			vtx.weight[3] *= sumInverse;
		}
	}
}

void Writer::AddVertex(cStringR meshName, int idx)
{
	MeshData* mesh = FindMesh(meshName);
	if (!mesh)
		return;

	mesh->verts.emplace_back();
}

void Writer::SetVertexPosition(cStringR meshName, FbxVector4 vector)
{
	MeshData* mesh = FindMesh(meshName);
	if (!mesh)
		return;

	mesh->verts.back().pos[0] = (float)vector[0];
	mesh->verts.back().pos[1] = (float)vector[1];
	mesh->verts.back().pos[2] = (float)vector[2];
	
}

void Writer::SetVertexUV(cStringR meshName, FbxVector2 vector)
{
	MeshData* mesh = FindMesh(meshName);
	if (!mesh)
		return;

	mesh->verts.back().uv[0] = (float)vector[0];
	mesh->verts.back().uv[1] = 1.f - (float)vector[1];
	
}

void Writer::SetVertexNormal(cStringR meshName, FbxVector4 vector)
{
	MeshData* mesh = FindMesh(meshName);
	if (!mesh)
		return;

	mesh->verts.back().normal[0] = (float)vector[0];
	mesh->verts.back().normal[1] = (float)vector[1];
	mesh->verts.back().normal[2] = (float)vector[2];
	
}

void Writer::AddVertexBiNormals(cStringR meshName)
{
	MeshData* mesh = FindMesh(meshName);
	if (mesh)
		mesh->biNorms.emplace_back();
}

void Writer::SetVertexTangent(cStringR meshName, FbxVector4 vector)
{
	MeshData* mesh = FindMesh(meshName);
	if (!mesh)
		return;

	mesh->biNorms.back().tangent[0] = (float)vector[0];
	mesh->biNorms.back().tangent[1] = (float)vector[1];
	mesh->biNorms.back().tangent[2] = (float)vector[2];
}

void Writer::SetVertexBiTangent(cStringR meshName, FbxVector4 vector)
{
	MeshData* mesh = FindMesh(meshName);
	if (!mesh)
		return;

	mesh->biNorms.back().biTan[0] = (float)vector[0];
	mesh->biNorms.back().biTan[1] = (float)vector[1];
	mesh->biNorms.back().biTan[2] = (float)vector[2];
}

void Writer::SetMaterial(cStringR meshName, cStringR mtlName)
{
	MeshData* mesh = FindMesh(meshName);
	if (mesh)
		mesh->mtlName = mtlName;
}

void Writer::AddMaterial(cStringR name)
{
	for (Material& mat : materials)
		if (mat.matName == name)
			return;

	materials.emplace_back(name);
}

void Writer::SetMatAmbient(float ambient1, float ambient2, float ambient3)
{
	materials.back().ambient[0] = ambient1;
	materials.back().ambient[1] = ambient2;
	materials.back().ambient[2] = ambient3;
}

void Writer::SetMatDiffuse(float diffuse1, float diffuse2, float diffuse3)
{
	materials.back().diffuse[0] = diffuse1;
	materials.back().diffuse[1] = diffuse2;
	materials.back().diffuse[2] = diffuse3;
}

void Writer::SetMatSpecular(float specular1, float specular2, float specular3)
{
	materials.back().specular[0] = specular1;
	materials.back().specular[1] = specular2;
	materials.back().specular[2] = specular3;
}

void Writer::SetMatIdx(int idx)
{
	matIdx.emplace_back(idx);
}

void Writer::AddTexture(const char* path)
{
	/*std::string charString = path;
	std::size_t found = charString.find_last_of('/');
	charString = charString.substr(found + 1);

	for (Texture& tex : textures)
	{
		if (tex.textureName == charString)
			return;
	}
	textures.emplace_back(charString.c_str());*/
}

bool Writer::SetTexture(const char* materialName, const char* name)
{
	std::string charString = name;
	charString = charString.substr(charString.find_last_of('/') + 1);

	for (Material& mat : materials)
	{
		if (mat.matName == materialName)
		{
			mat.diffuseTexturePath = charString;
			return true;
		}
	}

	return false;
}

void Writer::CopyTextureFile(const char* texturePath)
{
	std::string path = texturePath;
	int pos = (int)path.find_last_of('/') + 1;
	path = path.substr(path.size() - (path.size() - pos));

	path = "../../Resources/JOYFiles/" + path;

	CopyFile(texturePath, path.c_str(), TRUE);
}

void Writer::AddCamera()
{
	cameras.emplace_back();
}

void Writer::SetCameraPos(FbxDouble3 camPos)
{
	cameras.back().pos[0] = (float)camPos[0];
	cameras.back().pos[1] = (float)camPos[1];
	cameras.back().pos[2] = (float)camPos[2];
}

void Writer::SetUpVector(FbxDouble3 upVec)
{
	cameras.back().upVec[0] = (float)upVec[0];
	cameras.back().upVec[1] = (float)upVec[1];
	cameras.back().upVec[2] = (float)upVec[2];
}

void Writer::SetFwVector(FbxDouble3 fwVec)
{
	cameras.back().fwVec[0] = (float)fwVec[0];
	cameras.back().fwVec[1] = (float)fwVec[1];
	cameras.back().fwVec[2] = (float)fwVec[2];
}

void Writer::SetFOV(float FOV)
{
	cameras.back().fov = FOV;
}

void Writer::SetNearPlane(float nearPlane)
{
	cameras.back().nearPlane = nearPlane;
}

void Writer::SetFarPlane(float farPlane)
{
	cameras.back().farPlane = farPlane;
}

void Writer::SetMorphFrames(cStringR meshName, FbxAnimCurve* pCurve, const char* curveName)
{
	if (!pCurve)
		return;

	MeshData* mesh = FindMesh(meshName);
	if (!mesh)
		return;

	MeshDeformer* pDeformer{};
	for (MeshDeformer* deformer : mesh->deformers)
	{
		if (deformer->shapeName == curveName)
		{
			pDeformer = deformer;
			break;
		}
	}
	if (!pDeformer)
		return;

	FbxTime fTime;
	int numFrames = pCurve->KeyGetCount();

	pDeformer->frames.reserve(numFrames);
	for (int i = 0; i < numFrames; i++)
	{
		fTime = pCurve->KeyGetTime(i);
		pDeformer->frames.emplace_back(int(fTime.GetSecondDouble() * 30.0), pCurve->KeyGetValue(i));
	}
}

void Writer::SetMorphVTX(FbxGeometry* pGeometry, FbxShape* pShape, const char* shapeName)
{
	std::string meshName = pGeometry->GetNode()->GetName();
	MeshData* mesh = FindMesh(meshName);
	if (!mesh)
		return;

	auto eleNormal = pShape->GetElementNormal(0);

	if (eleNormal->GetMappingMode() != FbxGeometryElement::eByControlPoint)
		return;

	FbxLayerElementArrayTemplate<FbxVector4>* lNormals = NULL;
	bool lStatus = pShape->GetNormals(&lNormals);
	if (!lStatus)
		return;

	mesh->deformers.emplace_back(new MeshDeformer);
	MeshDeformer& deformer = *mesh->deformers.back();
	deformer.shapeName = shapeName;

	deformer.vertices.reserve(pShape->GetControlPointsCount());

	FbxMesh* fbxMesh = pGeometry->GetNode()->GetMesh();
	int polycount = fbxMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pShape->GetControlPoints();

	for (int i = 0; i < polycount; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			int lControlPointIndex = fbxMesh->GetPolygonVertex(i, k);

			FbxVector4 point = lControlPoints[lControlPointIndex];
			FbxVector4 normal = lNormals->GetAt(lControlPointIndex);

			deformer.vertices.emplace_back();
			MorphVTX& mVtx = deformer.vertices.back();

			mVtx.pos[0] = (float)point[0];
			mVtx.pos[1] = (float)point[1];
			mVtx.pos[2] = (float)point[2];

			mVtx.normal[0] = (float)normal[0];
			mVtx.normal[1] = (float)normal[1];
			mVtx.normal[2] = (float)normal[2];
		}
	}

}

void Writer::AddLight()
{
	lights.emplace_back();
}

void Writer::SetLightColor(float col1, float col2, float col3)
{
	lights.back().color[0] = col1;
	lights.back().color[1] = col2;
	lights.back().color[2] = col3;
}

void Writer::SetLightPos(FbxDouble3 pos)
{
	if (lights.size() <= posIdx)
		return;

	lights[posIdx].pos[0] = (float)pos[0];
	lights[posIdx].pos[1] = (float)pos[1];
	lights[posIdx].pos[2] = (float)pos[2];
	posIdx++;
}

void Writer::SetLightDir(FbxDouble3 rot)
{
	if (lights.size() <= dirIdx)
		return;

	lights[dirIdx].dir[0] = (float)rot[0];
	lights[dirIdx].dir[1] = (float)rot[1];
	lights[dirIdx].dir[2] = (float)rot[2];
	dirIdx++;
}

void Writer::SetKeyframeValues(float value, std::string checker, int timeStamp, int idx, int idxAmount, std::string jointName)
{
	if (checker == "TX")
	{
		if (idx == 0)
		{
			for (int i = 0; i < idxAmount; i++)
			{
				keyframes.emplace_back();
			}
		}
		keyframes[instCounter + idx].frame.translate[0] = value;
		keyframes[instCounter + idx].frame.timeStamp = timeStamp;
		keyframes[instCounter + idx].jointName = jointName;
		if (idx == (idxAmount - 1))
		{
			isKeyframe = false;
		}
	}

	if (checker == "TY")
	{
		keyframes[instCounter + idx].frame.translate[1] = value;
		if (idx == (idxAmount - 1))
		{
			isKeyframe = false;
		}
	}
		
	if (checker == "TZ")
	{
		keyframes[instCounter + idx].frame.translate[2] = value;
		if (idx == (idxAmount - 1))
		{
			isKeyframe = false;
		}
	}

	if (checker == "RX")
	{
		keyframes[instCounter + idx].frame.rotate[0] = value;
		if (idx == (idxAmount - 1))
		{
			isKeyframe = false;
		}
	}

	if (checker == "RY")
	{
		keyframes[instCounter + idx].frame.rotate[1] = value;
		if (idx == (idxAmount - 1))
		{
			isKeyframe = false;
		}
	}

	if (checker == "RZ")
	{
		keyframes[instCounter + idx].frame.rotate[2] = value;
		if (idx == (idxAmount - 1))
		{
			isKeyframe = false;
		}
	}

	if (checker == "SX")
	{
		keyframes[instCounter + idx].frame.scale[0] = value;
		if (idx == (idxAmount - 1))
		{
			isKeyframe = false;
		}
	}

	if (checker == "SY")
	{
		keyframes[instCounter + idx].frame.scale[1] = value;
		if (idx == (idxAmount - 1))
		{
			isKeyframe = false;
		}
	}

	if (checker == "SZ")
	{
		keyframes[instCounter + idx].frame.scale[2] = value;
		if (idx == (idxAmount - 1))
		{
			instCounter += (idx + 1);
		}
		if (idx == (idxAmount - 1))
		{
			isKeyframe = false;
		}
	}
}

void Writer::KeyframeTrue()
{
	isKeyframe = true;
}

bool Writer::isKeyframeTrue()
{
	return isKeyframe;
}

void Writer::AddBindPose(cStringR name, FbxNode* pNode)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		for (int j = 0; j < meshes[i].jointNames.size(); j++)
		{
			if (meshes[i].jointNames[j] == name)
			{
				meshes[i].joints.emplace_back();
				meshes[i].joints.back().name = name;

				auto pParent = pNode->GetParent();
				if (pParent->GetNameOnly() != FbxString("RootNode"))
				{
					for (size_t k = 0; k < meshes[i].jointNames.size(); k++)
					{
						if (meshes[i].jointNames.at(k) == pParent->GetNameOnly().Buffer())
							meshes[i].joints.back().parentIdx = (int)k;
					}
				}
			}
		}
	}
}

void Writer::SetBindPose(float rowValue1, float rowValue2, float rowValue3, float rowValue4, int rowIdx, cStringR name)
{
	
	for (int i = 0; i < meshes.size(); i++)
	{
		for (int j = 0; j < meshes[i].jointNames.size(); j++)
		{
			if (meshes[i].jointNames[j] == name)
			{
				if (rowIdx == 0)
				{
					meshes[i].joints[bindPoseIdx].row1[0] = rowValue1;
					meshes[i].joints[bindPoseIdx].row1[1] = rowValue2;
					meshes[i].joints[bindPoseIdx].row1[2] = rowValue3;
					meshes[i].joints[bindPoseIdx].row1[3] = rowValue4;
				}
				if (rowIdx == 1)
				{
					meshes[i].joints[bindPoseIdx].row2[0] = rowValue1;
					meshes[i].joints[bindPoseIdx].row2[1] = rowValue2;
					meshes[i].joints[bindPoseIdx].row2[2] = rowValue3;
					meshes[i].joints[bindPoseIdx].row2[3] = rowValue4;
				}
				if (rowIdx == 2)
				{
					meshes[i].joints[bindPoseIdx].row3[0] = rowValue1;
					meshes[i].joints[bindPoseIdx].row3[1] = rowValue2;
					meshes[i].joints[bindPoseIdx].row3[2] = rowValue3;
					meshes[i].joints[bindPoseIdx].row3[3] = rowValue4;
				}
				if (rowIdx == 3)
				{
					meshes[i].joints[bindPoseIdx].row4[0] = rowValue1;
					meshes[i].joints[bindPoseIdx].row4[1] = rowValue2;
					meshes[i].joints[bindPoseIdx].row4[2] = rowValue3;
					meshes[i].joints[bindPoseIdx].row4[3] = rowValue4;
					bindPoseIdx++;
					if (bindPoseIdx == meshes[i].jointNames.size())
					{
						bindPoseIdx = 0;
					}
				}
			}
		}
	}
}

void Writer::CalcNumFrames()
{
	for (MeshData& mesh : meshes)
	{
		for (size_t i = 0; i < mesh.joints.size(); i++)
		{
			for (size_t k = 0; k < keyframes.size(); k++)
			{
				if (mesh.joints.at(i).name == keyframes.at(k).jointName)
					mesh.joints.at(i).numFrames++;
			}
		}
	}
}

void Writer::AddGroup(cStringR name)
{
	groups.emplace_back();
	groups.back().name = name;
	isGroup = true;
}

void Writer::SetGroupTranslation(FbxDouble3 translations)
{
	for (int i = 0; i < 3; i++)
	{
		groups.back().translate[i] = (float)translations[i];
	}
}

void Writer::SetGroupRotation(FbxDouble3 rotations)
{
	for (int i = 0; i < 3; i++)
	{
		groups.back().rotation[i] = (float)rotations[i];
	}
}

void Writer::SetGroupScale(FbxDouble3 scales)
{
	for (int i = 0; i < 3; i++)
	{
		groups.back().scale[i] = (float)scales[i];
	}
	isGroup = false;
}

bool Writer::isGroupTrue()
{
	return isGroup;
}

void Writer::SetAnimationTime(float frameRate, int start, int end)
{
	aniData.frameRate = frameRate;
	aniData.startFrame = start;
	aniData.endFrame = end;
}

void Writer::sortMaterial()
{
	int i, j, minIdx;
	int temp = (int)matIdx.size() - 1;
	for (i = 0; i < temp; i++)
	{
		minIdx = i;
		for (j = i + 1; j < matIdx.size(); j++)
		{
			if (matIdx[j] < matIdx[minIdx])
			{
				minIdx = j;
			}
			std::swap(matIdx[i], matIdx[minIdx]);
			std::swap(polygons[i], polygons[minIdx]);
		}
	}
}
