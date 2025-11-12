#include "StudentWorld.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}


int StudentWorld::init()
{
    mainPlayer = new Iceman(this);


    maxProtestors = min<unsigned int>(15, 2 + getLevel() * 1.5);
    protestorSpawnRate = max<unsigned int>(25, 200 - getLevel());




    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 60; j++) {

            if (i >= 30 && i <= 33 && j > 3) {

            }
            else {
                A[i][j] = new Ice(i, j, this);
            }

        }
    }


    boulderTotal = min<unsigned int>(getLevel() / 2 + 2, 9);
    for (int i = 0; i < boulderTotal; i++) {
        int x, y;
        for (;;) {
            x = rand() % 58;
            y = rand() % 37 + 20;
            if ((x <= 25 || x >= 35) && (y < 56) && notalreadyOccupied(x, y)) {
                break;
            }
        }
        gameActors.push_back(new Boulder(x, y, this));
        removeIce(x, y);

    }


    oilSpawn = min<unsigned int>(2 + getLevel(), 21);

    for (int i = 0; i < oilSpawn; i++) {
        int x, y;
        for (;;) {
            x = rand() % 60;
            y = rand() % 63;
            if ((x <= 25 || x >= 35) && (y < 56) && notalreadyOccupied(x, y)) {
                break;
            }
        }
        gameActors.push_back(new Oil(x, y, this));

    }


    int goldSpawn = max<unsigned int>(5 - getLevel() / 2, 2);

    for (int i = 0; i < goldSpawn; i++) {
        int x, y;
        for (;;) {
            x = rand() % 60;
            y = rand() % 63;
            if ((x <= 25 || x >= 35) && (y < 56) && notalreadyOccupied(x, y)) {
                break;
            }
        }
        gameActors.push_back(new Gold(x, y, this));

    }




    return GWSTATUS_CONTINUE_GAME;
}




int StudentWorld::move()
{

    displayText();
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    if (!mainPlayer->isLiving()) {
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    if (mainPlayer->isLiving()) {
        mainPlayer->doSomething();
    }

    for (int i = 0; i < gameProtestors.size(); i++) {
        if (gameProtestors[i]->isLiving()) {
            gameProtestors[i]->doSomething();
        }
    }

    for (int i = 0; i < gameActors.size(); i++) {
        if (gameActors[i]->isLiving()) {
            gameActors[i]->doSomething();
        }
    }




    if (maxProtestors > gameProtestors.size() && protestorSpawnRate >= max<unsigned int>(25, 200 - getLevel())) {
        int x = rand() % 100 + 1;
        if (x < min<unsigned int>(90, getLevel() * 10 + 30)) {
            gameProtestors.push_back(new Hardcore(this));
        }
        else {
            gameProtestors.push_back(new Normal(this));
        }
        protestorSpawnRate = 0;

    }
    protestorSpawnRate++;




    /*for (int i = 0; i < gameActors.size(); i++) {
        if (!gameActors[i]->isLiving()) {
            delete gameActors[i];
            gameActors[i] = nullptr;
            gameActors.pop_back();
        }
    }*/

    gameActors.erase(remove_if(gameActors.begin(), gameActors.end(), [](auto it) { return !(it->isLiving()); }), gameActors.end());

    gameProtestors.erase(remove_if(gameProtestors.begin(), gameProtestors.end(), [](auto it) { return !(it->isLiving()); }), gameProtestors.end());
    /*for (int i = 0; i < gameProtestors.size(); i++) {
        if (!gameProtestors[i]->isLiving()) {
            delete gameProtestors[i];
            gameProtestors[i] = nullptr;
            gameProtestors.pop_back();
        }
    }*/



    int goodiePossibility = getLevel() * 25 + 1000;

    int SonarorWater = rand() % goodiePossibility;

    if (SonarorWater == 0) {
        int probability = rand() % 5;
        if (probability == 0) {
            gameActors.push_back(new Sonar(this));
        }
        else {
            int x, y;
            for (;;) {
                x = rand() % 61;
                y = rand() % 61;
                if (notalreadyOccupied(x, y) && A[x][y] == nullptr) {
                    bool emptySpotFound = true;
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            if (A[x + i][y + j] != nullptr) {
                                emptySpotFound = false;
                                break;
                            }
                        }
                        if (!emptySpotFound) {
                            break;
                        }
                    }
                    if (emptySpotFound) {
                        break;
                    }
                }
            }

            gameActors.push_back(new Water(x, y, this));
            
        }

    }


    if (oilSpawn == getIceman()->getOil()) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }

    return GWSTATUS_CONTINUE_GAME;

}





bool StudentWorld::ifPossible(const int& x, const int& y) {
    if (x > -1 && x < 61 && y > -1 && y < 61) {
        return true;
    }
    return false;
}



bool StudentWorld::iceSpot(int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (A[x + i][y + j] != nullptr) {
                return true;
            }
        }
    }
    return false;
}




void StudentWorld::removeIce(int const& x, int const& y) {

    for (int i = x; i < x + 4; i++) {
        for (int j = y; j < y + 4; j++) {
            if (A[i][j] != nullptr) {
                A[i][j]->setVisible(false);
                delete A[i][j];
                A[i][j] = nullptr;
                playSound(SOUND_DIG);

            }
        }
    }
}



bool StudentWorld::isClose(const int& x, const int& y, int dist) {
    int a = mainPlayer->getX() - x;
    int b = mainPlayer->getY() - y;
    int distanceSquared = a * a + b * b;
    return (distanceSquared <= dist * dist);
}


bool StudentWorld::protestorGold(int x, int y) {
    for (int i = 0; i < gameProtestors.size(); i++) {
        if (gameProtestors[i]->isLiving()) {
            int a = gameProtestors[i]->getX() - x;
            int b = gameProtestors[i]->getY() - y;
            if (a * a + b * b <= 9 && a * a + b * b != 1) {
                gameProtestors[i]->makeLeave();
                playSound(SOUND_PROTESTER_FOUND_GOLD);
                if (gameProtestors[i]->getProt() == 1) {
                    mainPlayer->bribedProtestorNorm();
                }
                if (gameProtestors[i]->getProt() == 2) {
                    mainPlayer->bribedProtestorHard();
                }
                return true;
            }
        }
    }
    return false;
}



void StudentWorld::bribingGold(int x, int y) {
    gameActors.push_back(new droppedNugg(x, y, this));

}




bool StudentWorld::notalreadyOccupied(int x, int y) {
    for (int i = 0; i < gameActors.size(); i++) {
        if (gameActors[i]->isLiving()) {
            int a = gameActors[i]->getX() - x;
            int b = gameActors[i]->getY() - y;
            int distanceSquared = a * a + b * b;
            if (distanceSquared <= 36) {
                return false;
            }
        }
    }
    return true;
}



void StudentWorld::sonarScan(int x, int y) {
    for (int i = 0; i < gameActors.size(); i++) {
        if (gameActors[i]->isLiving()) {
            int a = gameActors[i]->getX() - x;
            int b = gameActors[i]->getY() - y;
            int distanceSquared = a * a + b * b;
            if (distanceSquared <= 144) {
                gameActors[i]->setVisible(true);
            }
        }
    }
    playSound(SOUND_SONAR);
}






bool StudentWorld::boulderSpot(int x, int y) {
    for (int i = 0; i < gameActors.size(); i++) {

        if (gameActors[i]->isLiving() && gameActors[i]->isBoulder()) {
            int a = gameActors[i]->getX() + 2 - x - 2;
            int b = gameActors[i]->getY() + 2 - y - 2;
            if (a * a + b * b <= 9 && a * a + b * b != 1) {
                return true;
            }

        }

    }
    return false;
}




void StudentWorld::protestorHitByBoulder(int x, int y) {
    for (int i = 0; i < gameProtestors.size(); i++) {
        if (gameProtestors[i]->isLiving()) {
            int a = gameProtestors[i]->getX() - x;
            int b = gameProtestors[i]->getY() - y;
            if (a * a + b * b <= 9 && a * a + b * b != 1) {
                gameProtestors[i]->makeLeave();
                if (gameProtestors[i]->getProt() == 1) {
                    mainPlayer->boulderHitProtestorNorm();
                }
                if (gameProtestors[i]->getProt() == 2) {
                    mainPlayer->boulderHitProtestorHard();
                }
            }
        }
    }
}




bool StudentWorld::squirtProtestor(int x, int y) {
    for (int i = 0; i < gameProtestors.size(); i++) {
        if (gameProtestors[i]->isLiving()) {
            int a = gameProtestors[i]->getX() - x;
            int b = gameProtestors[i]->getY() - y;
            if (a * a + b * b <= 9) {
                gameProtestors[i]->decreaseLife(1);
                gameProtestors[i]->Stun();
                if (gameProtestors[i]->getHealth() == 0) {
                    gameProtestors[i]->makeLeave();
                    if (gameProtestors[i]->getProt() == 1) {
                        mainPlayer->normalGaveUp();
                    }
                    else if (gameProtestors[i]->getProt() == 2) {
                        mainPlayer->hardcoreGaveUp();
                    }
                }
                return true;
            }
        }
    }
    return false;
}




void StudentWorld::spray(int x, int y, GraphObject::Direction facing) {
    gameActors.push_back(new Squirt(x, y, facing, this));
    playSound(SOUND_PLAYER_SQUIRT);
}



bool StudentWorld::isAbleToShout(int x, int y, GraphObject::Direction facing) {
    int a = mainPlayer->getX();
    int b = mainPlayer->getY();

    if (facing == GraphObject::left && x <= a + 4 && x >= a && y >= b - 4 && y <= b + 4) {
        return true;
    }
    else if (facing == GraphObject::right && x >= a - 4 && x <= a && y >= b - 4 && y <= b + 4) {
        return true;
    }
    else if (facing == GraphObject::up && y >= b - 4 && y <= b && x >= a - 4 && x <= a + 4) {
        return true;
    }
    else if (facing == GraphObject::down && y <= b + 4 && y >= b && x >= a - 4 && x <= a + 4) {
        return true;
    }

    return false;
}



bool StudentWorld::isInSight(GraphObject::Direction& facing, Protestor* prot) {
    int playerX = mainPlayer->getX();
    int playerY = mainPlayer->getY();
    int protestorX = prot->getX();
    int protestorY = prot->getY();

    if (playerY == protestorY)
    {
        if (playerX > protestorX) 
        {
            for (int i = protestorX; i < playerX; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (A[i][protestorY + j] != nullptr || boulderSpot(i, protestorY + j))
                        return false;
                }
            }
            facing = GraphObject::Direction::right;
            return true;
        }
        else if (playerX < protestorX) 
        {
            for (int i = protestorX; i > playerX; i--)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (A[i][protestorY + j] != nullptr || boulderSpot(i, protestorY + j))
                        return false;
                }
            }
            facing = GraphObject::Direction::left;
            return true;
        }
    }
    else if (playerX == protestorX)
    {
        if (playerY > protestorY) 
        {
            for (int i = protestorY; i < playerY; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (A[protestorX + j][i] != nullptr || boulderSpot(protestorX + j, i))
                        return false;
                }
            }
            facing = GraphObject::Direction::up;
            return true;
        }
        else if (playerY < protestorY) 
        {
            for (int i = protestorY; i > playerY; i--)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (A[protestorX + j][i] != nullptr || boulderSpot(protestorX + j, i))
                        return false;
                }
            }
            facing = GraphObject::Direction::down;
            return true;
        }
    }

    return false;
}



GraphObject::Direction StudentWorld::getNewDirection(int x, int y, GraphObject::Direction& facing) {
    
    for (;;) {
        int a = rand() % 4;

        if (a == 0) {
            if (!iceSpot(x - 1, y) && !boulderSpot(x - 1, y) && ifPossible(x - 1, y)) {
                facing = GraphObject::left;
                return facing;
            }
            break;
        }

        if (a == 1) {
            if (!iceSpot(x + 1, y) && !boulderSpot(x + 1, y) && ifPossible(x + 1, y)) {
                facing = GraphObject::right;
                return facing;
            }
            break;
        }

        if (a == 2) {
            if (!iceSpot(x, y + 1) && !boulderSpot(x, y + 1) && ifPossible(x, y + 1)) {
                facing = GraphObject::up;
                return facing;

            }
            break;
        }

        if (a == 3) {
            if (!iceSpot(x, y - 1) && !boulderSpot(x, y - 1) && ifPossible(x, y - 1)) {
                facing = GraphObject::down;
                return facing;

            }
            break;
        }
    }
    return facing;

}



void::StudentWorld::displayText() {
    int level = getLevel();
    int lives = getLives();
    int health = mainPlayer->getLifePoints();
    int gold = mainPlayer->getGold();
    int barrelsLeft = oilSpawn - mainPlayer->getOil();
    int sonar = mainPlayer->getSonar();
    int squirts = mainPlayer->getWater();
    int score = mainPlayer->getScore();

    ostringstream oss;
    oss << "Lvl: " << setw(2) << level << "  Lives: " << lives << "  Hlth: " << setw(3) << health << "%" << "  Wtr: " << setw(2) << squirts << "  Gld: " << setw(2) << gold << "  Oil Left: " << setw(2) << barrelsLeft << "  Sonar: " << setw(2) << sonar << "  Scr: " << setfill('0') << setw(5) << score;
    string text = oss.str();
    setGameStatText(text);
}



void::StudentWorld::cleanUp() {




    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 60; j++) {
            if (A[i][j] != nullptr) {
                delete A[i][j];
                A[i][j] = nullptr;
            }
        }
    }
    delete mainPlayer;
    for (int i = 0; i < gameActors.size(); i++) {
        if (gameActors[i]->isLiving()) {
            gameActors[i]->setVisible(false);
        }
    }
    for (int i = 0; i < gameProtestors.size(); i++) {
        if (gameProtestors[i]->isLiving()) {
            gameProtestors[i]->setVisible(false);
        }
    }

    gameActors.erase(remove_if(gameActors.begin(), gameActors.end(), [](auto it) { return (it->isLiving()); }), gameActors.end());

    gameProtestors.erase(remove_if(gameProtestors.begin(), gameProtestors.end(), [](auto it) { return (it->isLiving()); }), gameProtestors.end());

}








// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


