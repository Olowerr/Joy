#pragma once

enum struct HeaderType { None, Model, Material, Texture };

struct SectionHeader
{
	SectionHeader(HeaderType type)
		: type(type) 
	{
	}

	HeaderType type;
};