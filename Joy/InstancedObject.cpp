#include "InstancedObject.h"

InstancedObject::InstancedObject()
{
}

InstancedObject::~InstancedObject()
{
}

void InstancedObject::Shutdown()
{
	transformSRV->Release();
	// Don't release vertexBuffer, meshStorage owns it
	// Don't release mtl, meshStorage owns it
}
