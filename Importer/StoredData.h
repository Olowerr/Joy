#pragma once
#include "JOY.h"
#include "BaseHeader.h"
#include <vector>
#include <fstream>

/*@brief
* Use LoadAll to load .joy files, all info will be stored in vectors in StoredData
* Get pointers to the info you request.
* Use the data needed for your specific case
* Unload all to clear vectors
*/

struct MorphInfo
{
	std::vector<JOY::MorphFrame> morphFrame;
	std::vector<JOY::MorphVTX> morphVTX;
	JOY::MorphTargetInfo morphTargetInfo;
};

struct ObjectJointToho
{
	JOY::Joint jointInfo;
	std::vector<JOY::KeyFrame> keyFrames;
};

struct ObjectInfo
{
	std::vector<JOY::JoyString> children;

	ObjectHeader objHeader;

	std::vector<JOY::Vertex> vertex;
	std::vector<JOY::BiNormals> biNormal;
	std::vector<JOY::WeightVTX> weights;
	std::vector<int> indices;

	std::vector<MorphInfo> mInfo;

	std::vector<ObjectJointToho> joints;

	std::vector<JOY::PropertyBase*> properties;
};

struct MaterialInfo
{
	std::vector<JOY::Material> material;
};

struct LightInfo
{
	LightInfo()
		: lights{} {}

	std::vector<JOY::Light> lights;
};

struct CameraInfo
{
	CameraInfo()
		: cameras{} {}

	std::vector<JOY::Camera> cameras;
};

struct GroupInfo
{
	GroupInfo()
		: groups{} {}

	std::vector<JOY::Group> groups;
};



class StoredData
{

public:
	static StoredData& GetInstance()
	{
		static StoredData data;
		return data;
	}

	StoredData();
	~StoredData();

	/**
	*@brief Load all data from fbx file
	*@param string with filepath to .joy file
	*@return bool true if succeded
	*/
	bool LoadAll(const std::string& fileName);

	/*
	*@brief Clear all vectors that have been used in LoadAll function
	*/
	void UnloadAll();

	/*
	*@return Returns light info
	*/
	LightInfo& GetLightInfo();

	/*
	*@return Returns camera info
	*/
	CameraInfo& GetCameraInfo();

	/*
	*@return Returns group info
	*/
	GroupInfo& GetGroupInfo();

	/*
	*@return Returns object info
	*/
	std::vector<ObjectInfo>& GetObjectInfo();

	/*
	*@return Returns object info
	*/
	MaterialInfo& GetMaterialInfo();

	/*
	*@param Object you want material info for
	*@return pointer to material info for object
	*/
	JOY::Material const* GetMaterial(ObjectInfo& object);

	/*
	*@return Send in object and its index in array. Returns pointer to its child
	*/
	ObjectInfo* GetChild(ObjectInfo& parent, size_t index);

	/*
	*@return Returns object info
	*/
	ObjectInfo* GetObjectByName(const std::string& name);


	std::vector<ObjectInfo> m_objectInfoVec;
	LightInfo m_lightInfo;
	CameraInfo m_cameraInfo;
	GroupInfo m_groupInfo;
	MaterialInfo m_materialInfo;

};