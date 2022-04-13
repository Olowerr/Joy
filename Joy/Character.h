#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Object.h"
#include "Transform.h"
#include <iostream>

namespace DX = DirectX;
class Character: public Object
{
public:
	Character(Mesh* mesh);
	void move();
private:
	Keyboard& key;
	float speed;
	float speed2;
	bool diagMove;
	float maxSpeed;
	float minSpeed;
	bool decreaseSpeed;
	bool decreaseSpeed2;

};