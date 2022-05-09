#pragma once
#include <string>

#undef max
#undef min
#define NOMINMAX

namespace JOY
{

	struct JoyString
	{
		JoyString() = default;
		~JoyString() = default;
		JoyString(const JoyString& other) = default;
		JoyString(JoyString&& other) = default;
		JoyString& operator=(const JoyString& other) = default;
		JoyString& operator=(const char* pChar)
		{
			memcpy(string, pChar, std::min((int)strlen(pChar), 63));
			return *this;
		}
		JoyString& operator=(const std::string& stdString)
		{
			memcpy(string, stdString.c_str(), std::min((int)stdString.size(), 63));
			return *this;
		}


		JoyString(const char* pChar)
		{
			memcpy(string, pChar, std::min((int)strlen(pChar), 63));
		}
		JoyString(const std::string& stdString)
		{
			memcpy(string, stdString.c_str(), std::min((int)stdString.size(), 63));
		}

		bool operator==(const JoyString& other)
		{
			for (unsigned int i = 0; i < 63; i++) // last always null terminator
			{
				if (string[i] != other.string[i])
					return false;
			}
			return true;
		}

		char string[64]{}; // 64th is null terminator
	};

	struct AnimationData
	{
		AnimationData() = default;

		float frameRate;
		int startFrame;
		int endFrame;
	};

	struct Vertex
	{
		Vertex() :pos{}, uv{}, normal{} { }
		Vertex(float* Pos, float* Uv, float* Normal)
		{
			for (size_t i = 0; i < 3; i++)
			{
				pos[i] = Pos[i];
				normal[i] = Normal[i];
			}

			uv[0] = Uv[0];
			uv[1] = Uv[1];
		}
		bool operator==(const Vertex& other) const
		{
			for (size_t i = 0; i < 3; i++)
			{
				if (pos[i] != other.pos[i])
					return false;
				if (normal[i] != other.normal[i])
					return false;
			}

			if (uv[0] != other.uv[0])
				return false;
			if (uv[1] != other.uv[1])
				return false;


			return true;
		}

		float pos[3];
		float normal[3];
		float uv[2];
	};

	struct BiNormals
	{
		BiNormals() : tangent{}, biTan{} {}
		bool operator==(const BiNormals& other)
		{
			for (size_t i = 0; i < 3; i++)
			{
				if (tangent[i] != other.tangent[i])
					return false;
				if (biTan[i] != other.biTan[i])
					return false;
			}
			return true;
		}

		float tangent[3];
		float biTan[3];
	};

	struct WeightVTX
	{
		WeightVTX() :boneIdx{ -1, -1, -1, -1 }, weight{} {	}
		bool operator==(const WeightVTX& other)
		{
			for (size_t i = 0; i < 4; i++)
			{
				if (boneIdx[i] != other.boneIdx[i])
					return false;
				if (weight[i] != other.weight[i])
					return false;
			}
			return true;
		}

		int boneIdx[4];
		float weight[4];
	};

	struct Material
	{
		Material()
			:matName(), ambient{}, diffuse{}, specular{}, diffuseTexturePath("None") { }
		Material(const std::string& name)
			:matName(name), ambient{}, diffuse{}, specular{}, diffuseTexturePath("None") { }

		JoyString matName;
		JoyString diffuseTexturePath;
		float ambient[3];
		float diffuse[3];
		float specular[3];
	};

	enum struct PropertyType
	{
		NONE,
		VECTOR,
		FLOAT,
		INT,
		BOOL,
		STRING,
		ENUM
	};

	struct PropertyBase
	{
		PropertyBase() :type(PropertyType::NONE) { };
		PropertyBase(PropertyType type) :type(type) { };
		PropertyType type;
	};

	template<typename T>
	struct Property : PropertyBase
	{
		Property(PropertyBase type) :PropertyBase(type), value() {}
		Property(PropertyBase type, T value) : PropertyBase(type), value(value) {}

		T value;
	};

	struct Camera
	{
		Camera() = default;

		float pos[3];
		float upVec[3];
		float fwVec[3];
		float fov;
		float nearPlane;
		float farPlane;
	};

	struct Light
	{
		Light() = default;

		float color[3];
		float pos[3];
		float dir[3];
	};

	struct KeyFrame
	{
		KeyFrame() = default;
		float translate[3];
		float rotate[3];
		float scale[3];
		int timeStamp;
	};

	struct Joint
	{
		Joint()
			:parentIdx(-1), numFrames(0),
			row1{ 1.f, 0.f, 0.f, 0.f }, row2{ 0.f, 1.f, 0.f, 0.f },
			row3{ 0.f, 0.f, 1.f, 0.f }, row4{ 0.f, 0.f, 0.f, 1.f }
		{ }

		JoyString name;
		int parentIdx;
		int numFrames;

		// Inverse Bindpose matrix
		float row1[4];
		float row2[4];
		float row3[4];
		float row4[4];
	};

	struct Group
	{
		Group()
			:translate{}, rotation{}, scale{} { }

		JoyString name;
		float translate[3];
		float rotation[3];
		float scale[3];
	};

	struct Float3
	{
		Float3() = default;
		Float3(float x, float y, float z)
			:x(x), y(y), z(z)
		{
		}

		float x;
		float y;
		float z;
	};

	struct MorphTargetInfo
	{
		JoyString name;
		int numFrames = 0;
	};

	struct MorphFrame
	{
		MorphFrame() = default;
		MorphFrame(int keyTime, float keyValue)
			:keyTime(keyTime), keyValue(keyValue)
		{ }
		int keyTime;
		float keyValue;
	};

	struct MorphVTX
	{
		MorphVTX() = default;
		float pos[3];
		float normal[3];
	};
}
