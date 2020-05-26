#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const double PI = 3.1415926;
class StudentWorld;

//class Actor --------------------------------------------------
class Actor: public GraphObject{
public:
    Actor(int imageID, double startX, double startY, Direction dir , int depth, double size, StudentWorld* sw);
    virtual ~Actor();
    bool isAlive();
    StudentWorld* getSW();
    virtual void doSomething()= 0;
    virtual bool canBlock()=0;
    virtual bool damageable()=0;
    virtual bool hasHitpoints();
    virtual bool setPoints(int points,int addOrDe);
    virtual bool canBeEaten()=0;
    virtual int getPoints();
    virtual void playSound(int HurtOrDie)=0;
    void setDead();
private:
    bool m_isAlive;
    StudentWorld* m_sw;
};

//class StationaryObjects --------------------------------------------
class StationaryObjects: public Actor{
public:
    StationaryObjects(int imageID, double startX, double startY, Direction dir, int depth, double size, StudentWorld* sw);
    virtual void playSound(int HurtOrDie);
    virtual void doSomething()=0;
    virtual bool canBlock();
    virtual bool damageable()=0;
    virtual bool canBeEaten();

};

//class Dirt
class Dirt: public StationaryObjects{
public:
    Dirt(double startX, double startY, StudentWorld* sw);
    virtual void doSomething();
    virtual bool canBlock();
    virtual bool damageable();
private:
    //block ability
};

//class Pit
class Pit:public StationaryObjects{
public:
    Pit(double startX, double startY,StudentWorld* sw);
    virtual void doSomething();
    virtual bool damageable();
    virtual void pickABacteriaToGenerate();
private:
    int m_nOfRS;
    int m_nOfAS;
    int m_nOfEC;
    int m_BacteriaLeft;
};

//class Food --------------------------------------------------
class Food:public StationaryObjects{
public:
    Food(double startX,double startY,StudentWorld* sw);
    virtual void doSomething();
    virtual bool damageable();
    virtual bool canBeEaten();
};

//class MayGoesAway --------------------------------------------------
class MayGoesAway:public StationaryObjects{
public:
    MayGoesAway(int imageID, double startX, double startY,StudentWorld* sw,int LifeTime);
    virtual void doSomething() = 0;
    bool damageable();
protected:
    int GetLifeTime();
    void DeLifeTime();
private:
    int m_lifeTime;
};

//class RestoreHealth
class RestoreHealth:public MayGoesAway{
public:
    RestoreHealth(double startX, double startY,StudentWorld* sw,int LifeTime);
    virtual void doSomething();};

//class FlameThrower
class FlameThrower:public MayGoesAway{
public:
    FlameThrower(double startX, double startY,StudentWorld* sw, int LifeTime);
    virtual void doSomething();};

//class ExtraLife
class ExtraLife:public MayGoesAway{
public:
     ExtraLife(double startX, double startY,StudentWorld* sw, int LifeTime);
    virtual void doSomething();};


//class Fungus
class Fungus:public MayGoesAway{
public:
    Fungus(double startX, double startY,StudentWorld* sw, int LifeTime);
    void doSomething();
};

//class MovableObjects -----------------------------------------------
class MovableObjects:public Actor{
public:
    MovableObjects(int imageID, double startX, double startY, int points, Direction dir , int depth, double size, StudentWorld* sw);
    virtual void playSound(int HurtOrDie)=0;
    int getPoints();
    bool setPoints(int points,int addOrDe);
    bool canBlock();
    virtual bool damageable()=0;
    bool hasHitpoints();
    bool canBeEaten();
private:
    int m_hitPoints;
};

//class Bacteria -------------------------------------------------------
class Bacteria: public MovableObjects{
public:
    Bacteria(int imageID, double startX, double startY, int points,int damage, StudentWorld* sw);
    void doSomething();
    virtual bool goForSocrate()=0;
    virtual void MoveWhenDisIsGreaterThan0();
    int getEatenFood();
    int getPlanDistance();
    virtual bool damageable();
    void CleanEatenBacteria();
    void deDistance();
    void resetDistance();
    virtual void playSound(int HurtOrDie)=0;
    int getDamage();
private:
   // virtual void doDifferentiatedThing();
    int m_planDistance;
    int m_foodEaten;
    int m_damage;
};

//class RSmna  --------------------------------------------------------
class RSmna:public Bacteria{
public:
    RSmna(double startX, double startY,StudentWorld* sw);
    virtual void playSound(int HurtOrDie);
    virtual bool goForSocrate();
private:
    //virtual void doDifferentiatedThing();
};

//class ASmna---------------------------------------------
class ASmna:public Bacteria{
public:
    ASmna(double startX, double startY, StudentWorld* sw);
    virtual void playSound(int HurtOrDie);
    virtual bool goForSocrate();
};

//class EC
class EC:public Bacteria{
public:
    EC(double startX, double startY, StudentWorld* sw);
    virtual void playSound(int HurtOrDie);
    virtual bool goForSocrate();
    virtual void MoveWhenDisIsGreaterThan0();
};

//class Socrates   ------------------------------------------------------
class Socrates: public MovableObjects{
public:
    Socrates(StudentWorld* sw);
    virtual void doSomething();
    virtual bool damageable();
    void playSound(int HurtOrDie);
    int getNofSpray();
    int getNOfFlame();
    void setNOfFlame(int n);
private:
    int m_nOfSpray;
    int m_nOfFlame;
    int m_positionAngel;
    int getAngel();
};

//class Projectile -----------------------------------------------------
class Projectile:public Actor{
public:
    Projectile(int imageID, double startX, double startY, Direction dir , int depth, double size, StudentWorld* sw, int travelDistance,int points);
    virtual void doSomething();
    void playSound(int HurtOrDie);
    bool canBlock();
    bool damageable();
    virtual bool canBeEaten();
private:
    int m_travelDistance;
    int points;
    int getTravelDistance();
    void DeTravelDistance(int num);
    
};

//class Flame
class Flame: public Projectile{
public:
    Flame(double startX, double startY, Direction dir,StudentWorld* sw);
};

//class Spray
class Spray:public Projectile{
public:
    Spray(double startX, double startY, Direction dir,StudentWorld* sw);
};

#endif // ACTOR_H_
