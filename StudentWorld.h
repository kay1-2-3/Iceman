#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <memory>
#include <ctime>
#include <algorithm>
#include <utility>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	~StudentWorld() {
		cleanUp();
	}

	Iceman* getIceman() { return mainPlayer; }

	bool ifPossible(const int& x, const int& y);

	bool iceSpot(int x, int y);

	bool isClose(const int& x, const int& y, int dist);

	bool notalreadyOccupied(int x, int y);

	void removeIce(int const& x, int const& y);

	void sonarScan(int x, int y);

	bool squirtProtestor(int x, int y);

	bool protestorGold(int x, int y);

	void bribingGold(int x, int y);


	bool boulderSpot(int x, int y);

	void spray(int x, int y, GraphObject::Direction facing);

	void protestorHitByBoulder(int x, int y);

	bool isAbleToShout(int x, int y, GraphObject::Direction facing);

	bool isInSight(GraphObject::Direction& facing, Protestor* prot);

	GraphObject::Direction getNewDirection(int x, int y, GraphObject::Direction& facing);

	void displayText();


private:

	std::vector<Actor*> gameActors;
	std::vector<Protestor*> gameProtestors;
	int protTotal = 0;
	int oilSpawn;
	Ice* A[64][64]{ nullptr };
	int boulderTotal;
	Iceman* mainPlayer;

	int maxProtestors;
	int protestorSpawnRate;



};

#endif // STUDENTWORLD_H_








