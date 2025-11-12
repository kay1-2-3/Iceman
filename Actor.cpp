#include "Actor.h"
#include "StudentWorld.h"

#include<iostream>
#include<vector>
#include <queue>
#include <utility>
using namespace std;


//bool ifPossible(const int& x, const int& y) {
//	if (x > -1 && x < 61 && y > -1 && y < 61) {
//		return true;
//	}
//	return false;
//}


void Iceman::doSomething() {
	if (!isLiving()) {
		return;
	}


	getGame()->removeIce(getX(), getY());

	int input;
	if (getGame()->getKey(input)) {

		if (input == KEY_PRESS_ESCAPE) {
			makeDead();
		}


		if (input == KEY_PRESS_TAB) {
			if (goldCount > 0) {
				if (getDirection() == right && getX() < 60) {
					if (!getGame()->iceSpot(getX() - 4, getY()) && !getGame()->boulderSpot(getX() - 4, getY()) && getGame()->ifPossible(getX() - 4, getY())) {
						getGame()->bribingGold(getX() - 4, getY());
						goldCount--;
					}
				}
				else if (getDirection() == left && getX() > 0) {
					if (!getGame()->iceSpot(getX() + 4, getY()) && !getGame()->boulderSpot(getX() + 4, getY()) && getGame()->ifPossible(getX() + 4, getY())) {
						getGame()->bribingGold(getX() + 4, getY());
						goldCount--;
					}
				}
				else if (getDirection() == up && getY() < 60) {
					if (!getGame()->iceSpot(getX(), getY() - 4) && !getGame()->boulderSpot(getX(), getY() - 4) && getGame()->ifPossible(getX(), getY() - 4)) {
						getGame()->bribingGold(getX(), getY() - 4);
						goldCount--;
					}
				}
				else if (getDirection() == down && getY() > 0) {
					if (!getGame()->iceSpot(getX(), getY() + 4) && !getGame()->boulderSpot(getX(), getY() + 4) && getGame()->ifPossible(getX(), getY() + 4)) {
						getGame()->bribingGold(getX(), getY() + 4);
						goldCount--;
					}
				}
			}
		}



		if (input == KEY_PRESS_SPACE) {
			if (waterCount > 0) {
				if (getDirection() == right && getX() < 60) {
					getGame()->spray(getX() + 4, getY(), right);
					waterCount--;
				}

				else if (getDirection() == left && getX() > 0) {
					getGame()->spray(getX() - 4, getY(), left);
					waterCount--;
				}

				else if (getDirection() == up && getY() < 60) {
					getGame()->spray(getX(), getY() + 4, up);
					waterCount--;
				}

				else if (getDirection() == down && getY() > 0) {
					getGame()->spray(getX(), getY() - 4, down);
					waterCount--;
				}
			}
		}

		if (input == KEY_PRESS_UP) {
			setDirection(up);

			if (getGame()->ifPossible(getX(), getY() + 1) && !getGame()->boulderSpot(getX(), getY() + 1)) {
				moveTo(getX(), getY() + 1);

			}
		}

		if (input == KEY_PRESS_DOWN) {
			setDirection(down);

			if (getGame()->ifPossible(getX(), getY() - 1) && !getGame()->boulderSpot(getX(), getY() - 1)) {
				moveTo(getX(), getY() - 1);

			}
		}

		if (input == KEY_PRESS_RIGHT) {
			setDirection(right);

			if (getGame()->ifPossible(getX() + 1, getY()) && !getGame()->boulderSpot(getX() + 1, getY())) {
				moveTo(getX() + 1, getY());

			}
		}

		if (input == KEY_PRESS_LEFT) {
			setDirection(left);

			if (getGame()->ifPossible(getX() - 1, getY()) && !getGame()->boulderSpot(getX() - 1, getY())) {
				moveTo(getX() - 1, getY());

			}
		}

		if (input == 'Z' || input == 'z') {
			if (sonarCount > 0) {
				sonarCount--;
				getGame()->sonarScan(getX(), getY());
			}
		}





	}
}


void Protestor::shout() {
	if (nonRestingTicksAfterShouting <= 0) {
		getGame()->playSound(SOUND_PROTESTER_YELL);
		getGame()->getIceman()->gotHurt(20);
		if (getGame()->getIceman()->getLifePoints() == 0) {
			getGame()->getIceman()->makeDead();
		}
		resetShoutTicks();
	}
}







bool Protestor::escape(int x, int y, int xend, int yend, Direction& facing) {
	const int UNEXPLORED = 9999;
	int stepsArray[64][64];
	bool rv = false;
	for (int i = 0; i != 64; i++) {
		for (int j = 0; j != 64; j++) {
			stepsArray[i][j] = UNEXPLORED;
		}
	}

	std::queue<std::pair<int, int>> exitPath;
	exitPath.push(std::make_pair(xend, yend));
	stepsArray[xend][yend] = 0;

	int curSteps;
	int curX, curY;

	while (!exitPath.empty()) {
		curX = exitPath.front().first;
		curY = exitPath.front().second;
		exitPath.pop();
		if (curX == x && curY == y) {
			rv = true;
		}

		curSteps = stepsArray[curX][curY];
		curSteps++;


		// Check neighboring cells
		if (curX < 60 && !getGame()->boulderSpot(curX + 1, curY) && !getGame()->iceSpot(curX + 1, curY) && stepsArray[curX + 1][curY] == UNEXPLORED) {
			exitPath.push(std::make_pair(curX + 1, curY));
			stepsArray[curX + 1][curY] = curSteps;
		}

		if (curX > 0 && !getGame()->boulderSpot(curX - 1, curY) && !getGame()->iceSpot(curX - 1, curY) && stepsArray[curX - 1][curY] == UNEXPLORED) {
			exitPath.push(std::make_pair(curX - 1, curY));
			stepsArray[curX - 1][curY] = curSteps;
		}

		if (curY < 60 && !getGame()->boulderSpot(curX, curY + 1) && !getGame()->iceSpot(curX, curY + 1) && stepsArray[curX][curY + 1] == UNEXPLORED) {
			exitPath.push(std::make_pair(curX, curY + 1));
			stepsArray[curX][curY + 1] = curSteps;
		}

		if (curY > 0 && !getGame()->boulderSpot(curX, curY - 1) && !getGame()->iceSpot(curX, curY - 1) && stepsArray[curX][curY - 1] == UNEXPLORED) {
			exitPath.push(std::make_pair(curX, curY - 1));
			stepsArray[curX][curY - 1] = curSteps;
		}
	}

	

	int minSteps = UNEXPLORED;

	if (x < 60 && stepsArray[x + 1][y] < minSteps) {
		minSteps = stepsArray[x + 1][y];
		facing = GraphObject::right;
	}

	if (x > 0 && stepsArray[x - 1][y] < minSteps) {
		minSteps = stepsArray[x - 1][y];
		facing = GraphObject::left;
	}

	if (y < 60 && stepsArray[x][y + 1] < minSteps) {
		minSteps = stepsArray[x][y + 1];
		facing = GraphObject::up;
	}

	if (y > 0 && stepsArray[x][y - 1] < minSteps) {
		minSteps = stepsArray[x][y - 1];
		facing = GraphObject::down;
	}

	return rv;
}





void::Normal::doSomething() {
	if (!isLiving()) {
		return;
	}

	ticksWait--;
	increaseTurningTicks();
	decreaseStun();

	if (stunCount() > 0) {
		return;
	}


	if (ticksWait > 0) {
		return;
	}



	if (status() == "leaveOilField") {
		if (getX() == 60 && getY() == 60) {
			setVisible(false);
			makeDead();
		}
		Direction z;
		if (escape(getX(), getY(), 60, 60, z));
			setDirection(z);
			if (getDirection() == up) {
				moveTo(getX(), getY() + 1);
				ticksWait = 20;
			}
			else if (getDirection() == down) {
				moveTo(getX(), getY() - 1);
				ticksWait = 20;
			}
			else if (getDirection() == left) {
				moveTo(getX() - 1, getY());
				ticksWait = 20;
			}
			else if (getDirection() == right) {
				moveTo(getX() + 1, getY());
				ticksWait = 20;
			}
		return;
	}



	if (ticksWait <= 0) {
		makeMove();
		decreaseShoutTicks();
	}


	if (status() == "rest") {
		return;
	}

	if (status() == "move") {
		decreaseShoutTicks();
		if (getGame()->isAbleToShout(getX(), getY(), getDirection())) {
			if (getDirection() == left) {
				if (getGame()->getIceman()->getX() < getX()) {
					shout();
				}
			}
			if (getDirection() == right) {
				if (getGame()->getIceman()->getX() > getX()) {
					shout();
				}
			}
			if (getDirection() == up) {
				if (getGame()->getIceman()->getY() > getY()) {
					shout();
				}
			}
			if (getDirection() == down) {
				if (getGame()->getIceman()->getY() < getY()) {
					shout();
				}
			}
			ticksWait = 20;
			increaseTurningTicks();
			makeRest();
			return;
		}

		GraphObject::Direction k;
		if (getGame()->isInSight(k, this)) {
			setDirection(k);
			if (k == left) {
				moveTo(getX() - 1, getY());
			}
			if (k == right) {
				moveTo(getX() + 1, getY());
			}
			if (k == up) {
				moveTo(getX(), getY() + 1);
			}
			if (k == down) {
				moveTo(getX(), getY() - 1);
			}
			ticksWait = 20;
			increaseTurningTicks();
			makeRest();
			return;
		}
		else {
			decreaseStepCount();
		}


		if (getStepsLeft() <= 0) {
			Direction a;
			getGame()->getNewDirection(getX(), getY(), a);
			increaseTurningTicks();
			setDirection(a);
			resetSquaresToMove();
		}
		else if (getTurningTicks() > 200) {
			if (getDirection() == up || getDirection() == down) {
				if (!getGame()->iceSpot(getX() + 1, getY()) && !getGame()->boulderSpot(getX() + 1, getY()) && getGame()->ifPossible(getX() + 1, getY())) {
					setDirection(right);
					resetSquaresToMove();
					resetTurningTicks();
					makeRest();
				}
				else if (!getGame()->iceSpot(getX() - 1, getY()) && !getGame()->boulderSpot(getX() - 1, getY()) && getGame()->ifPossible(getX() - 1, getY())) {
					setDirection(left);
					resetSquaresToMove();
					resetTurningTicks();
					makeRest();
				}
			}
			else if (!getDirection() == left || getDirection() == right) {
				if (getGame()->iceSpot(getX(), getY() + 1) && !getGame()->boulderSpot(getX(), getY() + 1) && getGame()->ifPossible(getX(), getY() + 1)) {
					setDirection(up);
					resetSquaresToMove();
					resetTurningTicks();
					makeRest();
				}
				else if (!getGame()->iceSpot(getX(), getY() - 1) && !getGame()->boulderSpot(getX(), getY() - 1) && getGame()->ifPossible(getX(), getY() - 1)) {
					setDirection(down);
					resetSquaresToMove();
					resetTurningTicks();
					makeRest();
				}
			}

		}

		if (getDirection() == left) {
			if (!getGame()->iceSpot(getX() - 1, getY()) && !getGame()->boulderSpot(getX() - 1, getY()) && getGame()->ifPossible(getX() - 1, getY())) {
				moveTo(getX() - 1, getY());
				decreaseStepCount();
				ticksWait = 20;
				makeRest();
				
			}
			else {
				decreaseStepCount();
				getGame()->getNewDirection(getX(), getY(), d);
				setDirection(d);
				ticksWait = 20;
				makeRest();
				

			}
		}
		else if (getDirection() == right) {
			if (!getGame()->iceSpot(getX() + 1, getY()) && !getGame()->boulderSpot(getX() + 1, getY()) && getGame()->ifPossible(getX() + 1, getY())) {
				moveTo(getX() + 1, getY());
				decreaseStepCount();
				ticksWait = 20;
				makeRest();
				
			}
			else {
				decreaseStepCount();
				getGame()->getNewDirection(getX(), getY(), d);
				setDirection(d);
				ticksWait = 20;
				makeRest();
				
			}
		}
		else if (getDirection() == up) {
			if (!getGame()->iceSpot(getX(), getY() + 1) && !getGame()->boulderSpot(getX(), getY() + 1) && getGame()->ifPossible(getX(), getY() + 1)) {
				moveTo(getX(), getY() + 1);
				decreaseStepCount();
				ticksWait = 20;
				makeRest();
				
			}
			else {
				decreaseStepCount();
				getGame()->getNewDirection(getX(), getY(), d);
				setDirection(d);
				ticksWait = 20;
				makeRest();
				
			}
		}
		else if (getDirection() == down) {
			if (!getGame()->iceSpot(getX(), getY() - 1) && !getGame()->boulderSpot(getX(), getY() - 1) && getGame()->ifPossible(getX(), getY() - 1)) {
				moveTo(getX(), getY() - 1);
				decreaseStepCount();
				ticksWait = 20;
				makeRest();
				
			}
			else {
				decreaseStepCount();
				getGame()->getNewDirection(getX(), getY(), d);
				setDirection(d);
				ticksWait = 20;
				makeRest();
				
			}
		}



	}




}




void::Hardcore::doSomething() {
	if (!isLiving()) {
		return;
	}

	ticksWait--;
	increaseTurningTicks();
	decreaseStun();

	if (stunCount() > 0) {
		return;
	}


	if (ticksWait > 0) {
		return;
	}



	while (getGame()->isClose(getX(), getY(), 16)) {
		if (getGame()->isClose(getX(), getY(), 3)) {
			break;
		}

		Direction z;
		if (escape(getX(), getY(), getGame()->getIceman()->getX(), getGame()->getIceman()->getY(), z));
		setDirection(z);
		if (getDirection() == up) {
			moveTo(getX(), getY() + 1);
			ticksWait = 20;
		}
		else if (getDirection() == down) {
			moveTo(getX(), getY() - 1);
			ticksWait = 20;
		}
		else if (getDirection() == left) {
			moveTo(getX() - 1, getY());
			ticksWait = 20;
		}
		else if (getDirection() == right) {
			moveTo(getX() + 1, getY());
			ticksWait = 20;
		}

		return;
	}





	if (status() == "leaveOilField") {
		if (getX() == 60 && getY() == 60) {
			setVisible(false);
			makeDead();
		}
		Direction z;
		if (escape(getX(), getY(), 60, 60, z));
		setDirection(z);
		if (getDirection() == up) {
			moveTo(getX(), getY() + 1);
			ticksWait = 20;
		}
		else if (getDirection() == down) {
			moveTo(getX(), getY() - 1);
			ticksWait = 20;
		}
		else if (getDirection() == left) {
			moveTo(getX() - 1, getY());
			ticksWait = 20;
		}
		else if (getDirection() == right) {
			moveTo(getX() + 1, getY());
			ticksWait = 20;
		}
		return;
	}







	if (ticksWait <= 0) {
		makeMove();
		decreaseShoutTicks();
	}


	if (status() == "rest") {
		return;
	}

	if (status() == "move") {
		decreaseShoutTicks();
		if (getGame()->isAbleToShout(getX(), getY(), getDirection())) {
			if (getDirection() == left) {
				if (getGame()->getIceman()->getX() < getX()) {
					shout();
				}
			}
			if (getDirection() == right) {
				if (getGame()->getIceman()->getX() > getX()) {
					shout();
				}
			}
			if (getDirection() == up) {
				if (getGame()->getIceman()->getY() > getY()) {
					shout();
				}
			}
			if (getDirection() == down) {
				if (getGame()->getIceman()->getY() < getY()) {
					shout();
				}
			}
			ticksWait = 20;
			increaseTurningTicks();
			makeRest();
			return;
		}

		GraphObject::Direction k;
		if (getGame()->isInSight(k, this)) {
			setDirection(k);
			if (k == left) {
				moveTo(getX() - 1, getY());
			}
			if (k == right) {
				moveTo(getX() + 1, getY());
			}
			if (k == up) {
				moveTo(getX(), getY() + 1);
			}
			if (k == down) {
				moveTo(getX(), getY() - 1);
			}
			ticksWait = 20;
			increaseTurningTicks();
			makeRest();
			return;
		}
		else {
			decreaseStepCount();
		}


		if (getStepsLeft() <= 0) {
			Direction a;
			getGame()->getNewDirection(getX(), getY(), a);
			increaseTurningTicks();
			setDirection(a);
			resetSquaresToMove();
		}
		else if (getTurningTicks() > 200) {
			if (getDirection() == up || getDirection() == down) {
				if (!getGame()->iceSpot(getX() + 1, getY()) && !getGame()->boulderSpot(getX() + 1, getY()) && getGame()->ifPossible(getX() + 1, getY())) {
					setDirection(right);
					resetSquaresToMove();
					resetTurningTicks();
					makeRest();
				}
				else if (!getGame()->iceSpot(getX() - 1, getY()) && !getGame()->boulderSpot(getX() - 1, getY()) && getGame()->ifPossible(getX() - 1, getY())) {
					setDirection(left);
					resetSquaresToMove();
					resetTurningTicks();
					makeRest();
				}
			}
			else if (!getDirection() == left || getDirection() == right) {
				if (getGame()->iceSpot(getX(), getY() + 1) && !getGame()->boulderSpot(getX(), getY() + 1) && getGame()->ifPossible(getX(), getY() + 1)) {
					setDirection(up);
					resetSquaresToMove();
					resetTurningTicks();
					makeRest();
				}
				else if (!getGame()->iceSpot(getX(), getY() - 1) && !getGame()->boulderSpot(getX(), getY() - 1) && getGame()->ifPossible(getX(), getY() - 1)) {
					setDirection(down);
					resetSquaresToMove();
					resetTurningTicks();
					makeRest();
				}
			}

		}

		if (getDirection() == left) {
			if (!getGame()->iceSpot(getX() - 1, getY()) && !getGame()->boulderSpot(getX() - 1, getY()) && getGame()->ifPossible(getX() - 1, getY())) {
				moveTo(getX() - 1, getY());
				decreaseStepCount();
				ticksWait = 20;
				makeRest();

			}
			else {
				decreaseStepCount();
				getGame()->getNewDirection(getX(), getY(), d);
				setDirection(d);
				ticksWait = 20;
				makeRest();


			}
		}
		else if (getDirection() == right) {
			if (!getGame()->iceSpot(getX() + 1, getY()) && !getGame()->boulderSpot(getX() + 1, getY()) && getGame()->ifPossible(getX() + 1, getY())) {
				moveTo(getX() + 1, getY());
				decreaseStepCount();
				ticksWait = 20;
				makeRest();

			}
			else {
				decreaseStepCount();
				getGame()->getNewDirection(getX(), getY(), d);
				setDirection(d);
				ticksWait = 20;
				makeRest();

			}
		}
		else if (getDirection() == up) {
			if (!getGame()->iceSpot(getX(), getY() + 1) && !getGame()->boulderSpot(getX(), getY() + 1) && getGame()->ifPossible(getX(), getY() + 1)) {
				moveTo(getX(), getY() + 1);
				decreaseStepCount();
				ticksWait = 20;
				makeRest();

			}
			else {
				decreaseStepCount();
				getGame()->getNewDirection(getX(), getY(), d);
				setDirection(d);
				ticksWait = 20;
				makeRest();

			}
		}
		else if (getDirection() == down) {
			if (!getGame()->iceSpot(getX(), getY() - 1) && !getGame()->boulderSpot(getX(), getY() - 1) && getGame()->ifPossible(getX(), getY() - 1)) {
				moveTo(getX(), getY() - 1);
				decreaseStepCount();
				ticksWait = 20;
				makeRest();

			}
			else {
				decreaseStepCount();
				getGame()->getNewDirection(getX(), getY(), d);
				setDirection(d);
				ticksWait = 20;
				makeRest();

			}
		}



	}




}












void::Ice::doSomething() {

}
void::Boulder::doSomething() {
	if (!isLiving()) {
		return;
	}

	if (fall > 0) {
		fall--;
		return;
	}

	if (!getGame()->iceSpot(getX(), getY() - 1)) {
		delay--;
	}

	if (time == 0 && delay <= 0) {
		getGame()->playSound(SOUND_FALLING_ROCK);
		time++;
	}

	if (delay <= 0) {
		if (!getGame()->iceSpot(getX(), getY() - 1) && !getGame()->boulderSpot(getX(), getY() - 1) && getGame()->ifPossible(getX(), getY() - 1)) {
			moveTo(getX(), getY() - 1);
			getGame()->protestorHitByBoulder(getX(), getY());
			fall = 7;
			if (getGame()->isClose(getX(), getY() - 1, 3)) {
				getGame()->getIceman()->makeDead();
			}
		}
		else {
			makeDead();
			setVisible(false);
		}
	}



}
void::Gold::doSomething() {

	if (!isVisible() && getGame()->isClose(getX(), getY(), 4)) {
		setVisible(true);
		return;
	}

	else if (getGame()->isClose(getX(), getY(), 3)) {
		makeDead();
		setVisible(false);
		getGame()->playSound(SOUND_GOT_GOODIE);
		getGame()->getIceman()->addGold();
		return;

	}
}



void::droppedNugg::doSomething() {
	if (lifeSpan == 0) {
		makeDead();
		return;
	}

	if (getGame()->protestorGold(getX(), getY())) {
		makeDead();
		return;
	}

	lifeSpan--;

}










void::Oil::doSomething() {


	if (!isVisible() && getGame()->isClose(getX(), getY(), 4)) {
		setVisible(true);
		return;
	}

	else if (getGame()->isClose(getX(), getY(), 3)) {
		makeDead();
		setVisible(false);
		getGame()->playSound(SOUND_FOUND_OIL);
		getGame()->getIceman()->addOil();
		return;

	}

}
void::Sonar::doSomething() {

	if (lifeSpan == 0) {
		makeDead();
		setVisible(false);
	}

	if (getGame()->isClose(getX(), getY(), 3)) {
		makeDead();
		setVisible(false);
		getGame()->playSound(SOUND_GOT_GOODIE);
		getGame()->getIceman()->addSonar();
		return;

	}
	lifeSpan--;
}


void::Water::doSomething() {

	if (lifeSpan == 0) {
		makeDead();
		setVisible(false);
	}

	if (getGame()->isClose(getX(), getY(), 3)) {
		makeDead();
		setVisible(false);
		getGame()->playSound(SOUND_GOT_GOODIE);
		getGame()->getIceman()->addWater();
		return;

	}
	lifeSpan--;
}





void::Squirt::doSomething() {

	if (!isLiving()) {
		return;
	}

	if (getGame()->squirtProtestor(getX(), getY())) {

	}

	if (distLeft == 0) {
		makeDead();
		return;
	}

	if (getDirection() == left) {
		if (getGame()->iceSpot(getX() - 1, getY()) || getGame()->boulderSpot(getX() - 1, getY()) || getX() <= 0) {
			makeDead();
		}
		else {
			distLeft--;
		}
	}
	else if (getDirection() == right) {
		if (getGame()->iceSpot(getX() + 1, getY()) || getGame()->boulderSpot(getX() + 1, getY()) || getX() >= 60) {
			makeDead();
		}
		else {
			distLeft--;
		}
	}
	else if (getDirection() == up) {
		if (getGame()->iceSpot(getX(), getY() + 1) || getGame()->boulderSpot(getX(), getY() + 1) || getX() >= 60) {
			makeDead();
		}
		else {
			distLeft--;
		}
	}
	else if (getDirection() == down) {
		if (getGame()->iceSpot(getX(), getY() - 1) || getGame()->boulderSpot(getX(), getY() - 1) || getX() <= 0) {
			makeDead();
		}
		else {
			distLeft--;
		}
	}



}


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

