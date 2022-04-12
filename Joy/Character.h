#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Object.h"
#include "Transform.h"

namespace DX = DirectX;
class Character: public Object
{
public:
	Character(Mesh* mesh);
	void move();
private:
	Keyboard& key;
	float speed;
	float maxSpeed;
	float minSpeed;
	float maxDiagSpeed;
	bool increaseSpeed;
	bool decreaseSpeed;
};