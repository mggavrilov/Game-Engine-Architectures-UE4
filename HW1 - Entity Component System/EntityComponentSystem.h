#ifndef ENTITYCOMPONENTSYSTEM_H
#define ENTITYCOMPONENTSYSTEM_H
#pragma once

#ifdef ENTITYCOMPONENTSYSTEM_EXPORTS  
#define ENTITYCOMPONENTSYSTEM_API __declspec(dllexport)   
#else  
#define ENTITYCOMPONENTSYSTEM_API __declspec(dllimport)   
#endif

#include <vector>
#include <string>
#include "Entity.h"
#include "Component.h"

class IWorld
{
	int frameIndex;
	std::string filename;
	std::vector<Entity> spawnedEntities;
	std::vector<Entity> availableEntities;
	std::vector<Coordinates> mushroomLocations;

public:
	IWorld();
	/// Reads the file specified by file
	/// and prepares the system for later calls to SpawnObject.
	/// All types described in the file must be spawnable.
	ENTITYCOMPONENTSYSTEM_API void ParseTypes(const char* file);
	/// Spawns a new object of the given type with the given name.
	ENTITYCOMPONENTSYSTEM_API void SpawnObject(const char* objectType, const char* objectName);
	/// Updates the world and all of its objects.
	/// Logs any information to the output file.
	ENTITYCOMPONENTSYSTEM_API void Update(float deltaMs);
	/// Destroys the world and all internal types, freeing all the memory.
	/// In its simplest form, it’s just Destroy() { delete this; }
	ENTITYCOMPONENTSYSTEM_API void Destroy();
};
/// Creates a new world. Remember PIMPL?
/// In its simplest, it’s just CreateWorld() { return new World(); }
extern "C" ENTITYCOMPONENTSYSTEM_API IWorld* CreateWorld();

#endif