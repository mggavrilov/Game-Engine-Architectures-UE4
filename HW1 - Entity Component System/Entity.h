#pragma once
#include <string>
#include <vector>
#include "Component.h"

struct Entity
{
	float x, y;
	std::string name;
	std::string type;
	std::vector<Component*> entityComponents;

	Entity();
	Entity(float x, float y, std::string name, std::string type);
	Entity(const Entity &t);
	Entity& operator=(const Entity &t);
	~Entity();
};