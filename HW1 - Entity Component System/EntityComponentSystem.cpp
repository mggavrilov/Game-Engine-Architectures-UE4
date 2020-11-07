// EntityComponentSystem.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "EntityComponentSystem.h"
#include <iostream>
#include <fstream>
#include <cmath>

IWorld::IWorld()
{
	frameIndex = 0;
}

IWorld* CreateWorld()
{
	return new IWorld();
}

void IWorld::ParseTypes(const char* file)
{
	filename = file;
	filename = filename.substr(0, filename.length() - 3);

	std::ifstream objectFile(file);

	std::string line;
	int lineCounter = 0;

	std::string type;

	while (std::getline(objectFile, line))
	{
		if (line[0] == 'c') //class
		{
			type = line.substr(6);
			
			Entity entity = Entity();
			entity.type = type;

			while (std::getline(objectFile, line))
			{
				line = line.substr(2);

				char component = line[0];

				if (component == 'R') //Renderable
				{
					Renderable* renderableComponent = new Renderable();
					entity.entityComponents.push_back(renderableComponent);
				}
				else
				{
					//(1, 2, 3)
					line = line.substr(line.find('(') + 1); // "1, 2, 3)"
					line = line.substr(0, line.length() - 1); // "1, 2, 3"

					if (component == 'J') //Jumpable
					{
						float height = std::stof(line.substr(0, line.find(',')));
						line = line.substr(line.find(',') + 2);
						float time = std::stof(line.substr(0, line.find(',')));
						line = line.substr(line.find(',') + 2);
						float delay = std::stof(line.substr(0, line.find(',')));

						Jumpable* jumpableComponent = new Jumpable(height, time, delay);
						entity.entityComponents.push_back(jumpableComponent);
					}
					else if (component == 'F') //FibonacciWalk
					{
						int maxFibIndex = std::stoi(line.substr(0, line.find(',')));
						line = line.substr(line.find(',') + 2);
						float sleepInterval = std::stof(line.substr(0, line.find(',')));
						line = line.substr(line.find(',') + 2);
						float sleepDuration = std::stof(line.substr(0, line.find(',')));
						
						FibonacciWalk* fibonacciComponent = new FibonacciWalk(maxFibIndex, sleepInterval, sleepDuration);
						entity.entityComponents.push_back(fibonacciComponent);
					}
					else if (component == 'S') //Shooter
					{
						std::string projectile = line.substr(0, line.find(','));
						float interval = std::stof(line.substr(line.find(',') + 2));

						Shooter* shooterComponent = new Shooter(projectile, interval);
						entity.entityComponents.push_back(shooterComponent);
					}
					else if (component == 'M') //Multiplier
					{
						Multiplier* multiplierComponent = new Multiplier();
						multiplierComponent->triggerDistance = std::stof(line.substr(line.find(']') + 2));

						std::string mushrooms = line.substr(1, line.find(']') - 1);

						while (mushrooms.length() > 0)
						{
							std::string current = mushrooms.substr(mushrooms.find('(') + 1);
							current = current.substr(0, current.find(')'));
							mushrooms.erase(0, mushrooms.find(')') + 1);

							Coordinates c;
							c.x = std::stof(current.substr(0, current.find(',')));
							c.y = std::stof(current.substr(current.find(',') + 1));

							mushroomLocations.push_back(c);
						}

						entity.entityComponents.push_back(multiplierComponent);
					}
					else
					{
						std::cerr << "Unknown component!";
						exit(1);
					}
				}

				if (objectFile.peek() == 'c') //end of components; new class
				{
					break;
				}
			}

			availableEntities.push_back(entity);
		}
		else
		{
			std::cerr << "Invalid text file format!";
			exit(1);
		}
	}
}

void IWorld::SpawnObject(const char* objectType, const char* objectName)
{
	int pos = -1;

	for (unsigned int i = 0; i < availableEntities.size(); ++i)
	{
		if (availableEntities[i].type.compare(objectType) == 0)
		{
			pos = i;
			break;
		}
	}

	if (pos != -1)
	{
		Entity entity = availableEntities[pos];
		entity.name = objectName;
		spawnedEntities.push_back(entity);
	}
	else
	{
		std::cerr << "Undefined class type!";
		exit(1);
	}
}

int fib(int n)
{
	if (n == 0 || n == 1)
		return 1;
	else
		return fib(n - 1) + fib(n - 2);
}

void IWorld::Update(float deltaMs)
{
	std::ofstream outputFile;
	outputFile.open(filename + ".out", std::ios::out | std::ios::app);

	outputFile << "--- Starting frame " << frameIndex << " ---" << std::endl;

	for (unsigned int i = 0; i < spawnedEntities.size(); ++i)
	{
		for (unsigned int j = 0; j < spawnedEntities[i].entityComponents.size(); ++j)
		{
			if (spawnedEntities[i].entityComponents[j]->componentType == RENDERABLE)
			{
				outputFile << spawnedEntities[i].name << " is located at (" << spawnedEntities[i].x << ", " << spawnedEntities[i].y << ")" << std::endl;
			}
			else if (spawnedEntities[i].entityComponents[j]->componentType == JUMPABLE)
			{
				Jumpable* tempComponent = static_cast<Jumpable*>(spawnedEntities[i].entityComponents[j]);

				if (tempComponent->asleep)
				{
					if (tempComponent->timePassed + deltaMs >= tempComponent->delay)
					{
						tempComponent->timePassed = deltaMs - (tempComponent->delay - tempComponent->timePassed);
						tempComponent->asleep = false;
						
						if (tempComponent->timePassed <= tempComponent->time) //jumping
						{
							spawnedEntities[i].y += tempComponent->timePassed / tempComponent->time * tempComponent->height;
						}
						else //falling down
						{
							spawnedEntities[i].y += (tempComponent->height - ((tempComponent->timePassed - tempComponent->time) / tempComponent->time * tempComponent->height));
						}
					}
					else
					{
						tempComponent->timePassed += deltaMs;
					}
				}
				else
				{
					if (tempComponent->timePassed + deltaMs >= tempComponent->time * 2)
					{
						tempComponent->asleep = true;
						spawnedEntities[i].y = 0; //fall to ground
						tempComponent->timePassed = deltaMs - (tempComponent->time - tempComponent->timePassed);
					}
					else
					{
						tempComponent->timePassed += deltaMs;

						if (tempComponent->timePassed <= tempComponent->time) //jumping
						{
							spawnedEntities[i].y = tempComponent->timePassed / tempComponent->time * tempComponent->height;
						}
						else //falling down
						{
							spawnedEntities[i].y = (tempComponent->height - ((tempComponent->timePassed - tempComponent->time) / tempComponent->time * tempComponent->height));
						}
					}
				}
			}
			else if (spawnedEntities[i].entityComponents[j]->componentType == FIBONACCI_WALK)
			{
				FibonacciWalk* tempComponent = static_cast<FibonacciWalk*>(spawnedEntities[i].entityComponents[j]);

				if (tempComponent->asleep)
				{
					if (tempComponent->timePassed + deltaMs >= tempComponent->sleepDuration)
					{
						spawnedEntities[i].x += fib(frameIndex % tempComponent->maxFibIndex);
						tempComponent->asleep = false;
						tempComponent->timePassed = deltaMs - (tempComponent->sleepDuration - tempComponent->timePassed);
					}
					else
					{
						tempComponent->timePassed += deltaMs;
					}
				}
				else
				{
					if (tempComponent->timePassed + deltaMs >= tempComponent->sleepInterval)
					{
						tempComponent->asleep = true;
						tempComponent->timePassed = deltaMs - (tempComponent->sleepInterval - tempComponent->timePassed);
					}
					else
					{
						tempComponent->timePassed += deltaMs;
						spawnedEntities[i].x += fib(frameIndex % tempComponent->maxFibIndex);
					}
				}
			}
			else if (spawnedEntities[i].entityComponents[j]->componentType == SHOOTER)
			{
				Shooter* tempComponent = static_cast<Shooter*>(spawnedEntities[i].entityComponents[j]);

				if (tempComponent->timePassed + deltaMs >= tempComponent->interval)
				{
					std::string projectileName = tempComponent->projectileName + std::to_string(tempComponent->nProjectiles);

					tempComponent->nProjectiles++;
					tempComponent->timePassed = deltaMs - (tempComponent->interval - tempComponent->timePassed);

					outputFile << spawnedEntities[i].name << " just shot " << projectileName << " - a projectile of type " << tempComponent->projectileName << std::endl;
				
					SpawnObject("Projectile", projectileName.c_str());
				}
				else
				{
					tempComponent->timePassed += deltaMs;
				}
			}
			else if (spawnedEntities[i].entityComponents[j]->componentType == MULTIPLIER)
			{
				Multiplier* tempComponent = static_cast<Multiplier*>(spawnedEntities[i].entityComponents[j]);

				float x1 = spawnedEntities[i].x;
				float y1 = spawnedEntities[i].y;
				
				for (unsigned int k = 0; k < mushroomLocations.size(); ++k)
				{
					float x2 = mushroomLocations[k].x;
					float y2 = mushroomLocations[k].y;

					if (sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)) <= tempComponent->triggerDistance)
					{
						outputFile << "A mushroom exploded!" << std::endl;
						mushroomLocations.erase(mushroomLocations.begin() + k);
						--k; //so it doesn't skip an entry

						for (unsigned int m = 0; m < spawnedEntities[i].entityComponents.size(); ++m)
						{
							if (spawnedEntities[i].entityComponents[m]->componentType == JUMPABLE)
							{
								Jumpable* tempComponent2 = static_cast<Jumpable*>(spawnedEntities[i].entityComponents[m]);
								tempComponent2->height *= 2;
							}
							else if (spawnedEntities[i].entityComponents[m]->componentType == FIBONACCI_WALK)
							{
								FibonacciWalk* tempComponent2 = static_cast<FibonacciWalk*>(spawnedEntities[i].entityComponents[m]);
								tempComponent2->maxFibIndex *= 2;
							}
						}
					}
				}
				
			}
		}
	}

	++frameIndex;
	outputFile.close();
}

void IWorld::Destroy()
{
	delete this;
}