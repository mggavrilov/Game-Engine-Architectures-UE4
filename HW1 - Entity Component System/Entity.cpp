#include "stdafx.h"
#include "Entity.h"

Entity::Entity()
{
	x = 0;
	y = 0;
	name = "";
	type = "";
}

Entity::Entity(float x, float y, std::string name, std::string type)
{
	this->x = x;
	this->y = y;
	this->name = name;
	this->type = type;
}

Entity::Entity(const Entity &t)
{
	x = t.x;
	y = t.y;
	name = t.name;
	type = t.type;

	for (unsigned int i = 0; i < t.entityComponents.size(); ++i)
	{
		if (t.entityComponents[i]->componentType == RENDERABLE)
		{
			entityComponents.push_back(new Renderable(*static_cast<Renderable*>(t.entityComponents[i])));
		}
		else if (t.entityComponents[i]->componentType == JUMPABLE)
		{
			entityComponents.push_back(new Jumpable(*static_cast<Jumpable*>(t.entityComponents[i])));
		}
		else if (t.entityComponents[i]->componentType == FIBONACCI_WALK)
		{
			entityComponents.push_back(new FibonacciWalk(*static_cast<FibonacciWalk*>(t.entityComponents[i])));
		}
		else if (t.entityComponents[i]->componentType == SHOOTER)
		{
			entityComponents.push_back(new Shooter(*static_cast<Shooter*>(t.entityComponents[i])));
		}
		else if (t.entityComponents[i]->componentType == MULTIPLIER)
		{
			entityComponents.push_back(new Multiplier(*static_cast<Multiplier*>(t.entityComponents[i])));
		}
	}
}

Entity& Entity::operator=(const Entity &t)
{
	if (this == &t)
		return *this;

	for (unsigned int i = 0; i < entityComponents.size(); ++i)
	{
		delete entityComponents[i];
	}

	entityComponents.clear();

	x = t.x;
	y = t.y;
	name = t.name;
	type = t.type;

	for (unsigned int i = 0; i < t.entityComponents.size(); ++i)
	{
		if (t.entityComponents[i]->componentType == RENDERABLE)
		{
			entityComponents.push_back(new Renderable(*static_cast<Renderable*>(t.entityComponents[i])));
		}
		else if (t.entityComponents[i]->componentType == JUMPABLE)
		{
			entityComponents.push_back(new Jumpable(*static_cast<Jumpable*>(t.entityComponents[i])));
		}
		else if (t.entityComponents[i]->componentType == FIBONACCI_WALK)
		{
			entityComponents.push_back(new FibonacciWalk(*static_cast<FibonacciWalk*>(t.entityComponents[i])));
		}
		else if (t.entityComponents[i]->componentType == SHOOTER)
		{
			entityComponents.push_back(new Shooter(*static_cast<Shooter*>(t.entityComponents[i])));
		}
		else if (t.entityComponents[i]->componentType == MULTIPLIER)
		{
			entityComponents.push_back(new Multiplier(*static_cast<Multiplier*>(t.entityComponents[i])));
		}
	}

	return *this;
}

Entity::~Entity()
{
	for (unsigned int i = 0; i < entityComponents.size(); ++i)
	{
		delete entityComponents[i];
	}
}