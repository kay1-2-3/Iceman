#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class StudentWorld;

class Actor : public GraphObject {
public:

	Actor(int displayID, int xcoord, int ycoord, Direction start, double image, int depth, int life, StudentWorld* newMap)
		:GraphObject(displayID, xcoord, ycoord, start, image, depth), isAlive(true), theWorld(newMap) {};

	virtual void doSomething() = 0;

	bool isLiving() const {
		return isAlive;
	}
	virtual void makeDead() {
		isAlive = false;
		setVisible(false);
	}

	StudentWorld* getGame() {
		return theWorld;
	}


	virtual bool isBoulder() { return false; }


	virtual ~Actor() {};

private:
	bool isAlive = true;
	StudentWorld* theWorld;


};



class Iceman : public Actor {
public:
	Iceman(StudentWorld* newMap) : Actor(IID_PLAYER, 30, 60, right, 1, 0, 10, newMap) {
		setVisible(true);
		lifePoints = 100;
		lifeTotal = 3;
		score = 0000;
	}

	int getLifePoints() { return lifePoints; }
	void gotHurt(int a) { lifePoints -= a; }
	int getOil() { return oilCount; }
	int getWater() { return waterCount; }
	int getSonar() { return sonarCount; }
	int getGold() { return goldCount; }
	int getScore() { return score; }


	void boulderHitProtestorNorm() { score += 500; }
	void boulderHitProtestorHard() { score += 750; }
	void normalGaveUp() { score += 100; }
	void hardcoreGaveUp() { score += 250; }

	void addOil() { oilCount += 1, score += 1000; }
	void addWater() { waterCount += 5, score += 100; }
	void addSonar() { sonarCount += 1, score += 75; }
	void addGold() { goldCount += 1, score += 10; }

	void bribedProtestorNorm() { score += 25; }
	void bribedProtestorHard() { score += 50; }




	virtual void doSomething();

	virtual ~Iceman() {};


private:
	int lifeTotal;
	int lifePoints;
	int oilCount = 0;
	int sonarCount = 1;
	int goldCount = 0;
	int waterCount = 5;
	int score;
};



class Protestor : public Actor {
public:
	Protestor(int displayID, int xcoord, int ycoord, Direction start, double image, int depth, StudentWorld* newMap)
		: Actor(displayID, xcoord, ycoord, start, image, depth, lifePoints, newMap) {
		m_state = "move";
		nonRestingTicksAfterShouting = 15;
		numSquaresToMove = rand() % 60 + 8;
		nonRestingTicksAfterTurn = 0;
	};

	virtual void doSomething() = 0;
	//virtual void getAnnoyed() = 0;
	//virtual void makeDead() = 0;
	virtual string status() { return m_state; }
	void makeLeave() { m_state = "leaveOilField"; }
	void makeMove() { m_state = "move"; }
	void makeRest() { m_state = "rest"; }
	void decreaseShoutTicks() { nonRestingTicksAfterShouting--; }
	void resetShoutTicks() { nonRestingTicksAfterShouting = 15; }
	void resetSquaresToMove() { numSquaresToMove = rand() % 52 + 8; }
	void decreaseStepCount() { numSquaresToMove--;}
	int getStepsLeft() { return numSquaresToMove; }
	void resetTurningTicks() { nonRestingTicksAfterTurn = 0; }
	void increaseTurningTicks() { nonRestingTicksAfterTurn++; }
	int getTurningTicks() { return nonRestingTicksAfterTurn; }

	void leavePath(int x, int y);

	bool escape(int x, int y , int xend, int yend, Direction& facing);

	int stunCount() { return stunTicks; }
	void Stun() { stunTicks = 500; }
	void decreaseStun() { stunTicks--; }

	void shout();

	int getProt() { return protValue; }

	void intValue(int a) { protValue = a; }
	void life(int health) { lifePoints = health; }
	int getHealth() { return lifePoints; }
	void decreaseLife(int minus) { lifePoints -= minus; }


	virtual ~Protestor() {};



	virtual string stateStatus() { return m_state; }

private:
	int stunTicks;
	int protValue;
	int lifePoints;
	string m_state;
	int numSquaresToMove;
	int nonRestingTicksAfterTurn;
	int nonRestingTicksAfterShouting;
	

};




class Normal : public Protestor {
public:
	Normal(StudentWorld* newMap) : Protestor(IID_PROTESTER, 60, 60, left, 1, 0, newMap) {
		setVisible(true);
		ticksWait = 20;
		life(12);
		intValue(1);
	}

	virtual void doSomething();

	void resetWaitTicks() { ticksWait = 50; }

	//virtual void getAnnoyed();
	//virtual void makeDead();




private:
	Direction d;
	int ticksWait;
	//int numSquaresToMove;
};


class Hardcore : public Protestor {
public:
	Hardcore(StudentWorld* newMap) : Protestor(IID_HARD_CORE_PROTESTER, 60, 60, left, 1, 0, newMap) {
		setVisible(true);
		ticksWait = 20;
		intValue(2);
		life(36);
	}

	virtual void doSomething();


private:
	int ticksWait;
	Direction d;
};












class Items : public Actor {
public:
	Items(int displayID, int xcoord, int ycoord, Direction start, double image, int depth, int life, StudentWorld* newMap)
		: Actor(displayID, xcoord, ycoord, start, image, depth, life, newMap) {};

	virtual void doSomething() = 0;
	virtual ~Items() {};



private:

};


class Ice : public Items {
public:
	Ice(int xcoord, int ycoord, StudentWorld* newMap) : Items(IID_ICE, xcoord, ycoord, right, 0.25, 3, 1, newMap) {
		setVisible(true);
	}

	virtual void doSomething() override;
	virtual ~Ice() {};



};


class Boulder : public Items {
public:
	Boulder(int xcoord, int ycoord, StudentWorld* newMap) : Items(IID_BOULDER, xcoord, ycoord, down, 1, 2, 1, newMap) {
		setVisible(true);
	}




	virtual bool isBoulder() { return true; }

	virtual void doSomething() override;
	virtual ~Boulder() {}

private:
	int delay = 100;
	int time = 0;
	int fall = 0;
};


class Gold : public Items {
public:
	Gold(int xcoord, int ycoord, StudentWorld* newMap) : Items(IID_GOLD, xcoord, ycoord, right, 1, 2, 1, newMap) {
		setVisible(false);
	}

	virtual void doSomething() override;
	virtual ~Gold() {}
};

class Oil : public Items {
public:
	Oil(int xcoord, int ycoord, StudentWorld* newMap) : Items(IID_BARREL, xcoord, ycoord, right, 1, 2, 1, newMap) {
		setVisible(false);
	}

	virtual void doSomething() override;
	virtual ~Oil() {}
};

class Sonar : public Items {
public:
	Sonar(StudentWorld* newMap) : Items(IID_SONAR, 0, 60, right, 1, 2, 1, newMap) {
		setVisible(true);
	}

	virtual void doSomething() override;
	virtual ~Sonar() {}
private:
	int lifeSpan = 1000;

};

class Water : public Items {
public:
	Water(int xcoord, int ycoord, StudentWorld* newMap) : Items(IID_WATER_POOL, xcoord, ycoord, right, 1, 2, 1, newMap) {
		setVisible(true);
	}

	virtual void doSomething() override;
	virtual ~Water() {}



private:
	int lifeSpan = 1000;
};


class Squirt : public Items {
public:
	Squirt(int xcoord, int ycoord, Direction start, StudentWorld* newMap) : Items(IID_WATER_SPURT, xcoord, ycoord, start, 1, 1, 1, newMap) {
		setVisible(true);
	}

	virtual void doSomething() override;
	virtual ~Squirt() {}

private:
	int distLeft = 4;
};



class droppedNugg : public Items {
public:
	droppedNugg(int xcoord, int ycoord, StudentWorld* newMap) : Items(IID_GOLD, xcoord, ycoord, right, 1, 2, 1, newMap) {
		setVisible(true);
	}

	virtual void doSomething() override;
	virtual ~droppedNugg() {}

private:
	int lifeSpan = 1000;
};



// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_


