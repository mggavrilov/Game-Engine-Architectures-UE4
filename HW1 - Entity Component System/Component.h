#pragma once
#include <string>
#include <vector>

//Components
#define RENDERABLE 1
#define JUMPABLE 2
#define FIBONACCI_WALK 3
#define SHOOTER 4
#define MULTIPLIER 5

struct Component
{
	int componentType;
};

struct Renderable : Component
{
	Renderable()
	{
		componentType = RENDERABLE;
	}
};

struct Jumpable : Component
{
	float height;
	float time;
	float delay;

	float timePassed;
	bool asleep;

	Jumpable(float h, float t, float d)
	{
		height = h;
		time = t * 1000; //convert to ms
		delay = d * 1000; //convert to ms
		componentType = JUMPABLE;

		timePassed = 0;
		asleep = false;
	}
};

struct FibonacciWalk : Component
{
	int maxFibIndex;
	float sleepInterval;
	float sleepDuration;

	float timePassed;
	bool asleep;

	FibonacciWalk(int m, float si, float sd)
	{
		maxFibIndex = m;
		sleepInterval = si * 1000; //convert to ms
		sleepDuration = sd * 1000; //convert to ms
		componentType = FIBONACCI_WALK;

		timePassed = 0;
		asleep = false;
	}
};

struct Shooter : Component
{
	std::string projectileName;
	float interval;
	
	float timePassed;
	int nProjectiles;

	Shooter(std::string p, float i)
	{
		projectileName = p;
		interval = i * 1000; //convert to ms
		componentType = SHOOTER;
	
		timePassed = 0;
		nProjectiles = 0;
	}
};

struct Coordinates //Helper
{
	float x;
	float y;
};

struct Multiplier : Component
{
	float triggerDistance;

	Multiplier()
	{
		componentType = MULTIPLIER;
	}
};