#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Socrates;
//class Socrates;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isObjectsOverlapped(double x1,double y1,double x2,double y2);
    bool ProjectileOverlapped(double x, double y,int points);
    bool isSocratesOverlapped(double x, double y);
    bool isFoodOverlapped(double x, double y);
    void setSocrateHealth(int amount, int dir);
    void chargeFlames();
    void addSpray();
    void addFlame();
    bool canMoveForward(double x, double y);
    bool isFoodFoundin128P(double x, double y);
    bool canMoveForwardDirt(double x, double y,Actor* bacteria,int distance);
    void increaseBac(int n);
    bool isSocrateFoundinNP(double x,double y,int z);
    void changeDirToFood(double x, double y,Actor* bacteria);
    void changeDirToSocrate(double x, double y,Actor* bacteria);
    void addRSmna(double x, double y);
    void addASmna(double x, double y);
    void addEC(double x, double y);
    

private:
    std::list<Actor*> objects;
    Socrates* player; 
    int m_remainingBacteria;
    void removeDeadObjects();
    void addNewActor();
    void updateDisplayText();
    void addGoodies();
    void addFungus();
    bool BacteriaOverlapped(double x1, double y1, double x2, double y2);
    double DistanceOf2Objects(double x1, double y1, double x2, double y2);
};


#endif // STUDENTWORLD_H_
