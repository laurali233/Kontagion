#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{m_remainingBacteria=0;}


int StudentWorld::init()
{
    player = new Socrates(this);
    
    for(int j =0; j<getLevel();j++){
    double x = rand()%VIEW_WIDTH;
    double y = rand()%VIEW_HEIGHT;
    double product = pow((x-VIEW_WIDTH/2),2)+pow(y-VIEW_HEIGHT/2,2);
    if(sqrt(product)<=120){
        list<Actor*>::iterator it1 = objects.begin();
        bool flag = true;
        for(;it1!=objects.end();it1++){
            if(isObjectsOverlapped(x, y,(*it1)->getX(), (*it1)->getY())){
                flag = false;
                break;}
        }
        if(flag){
            objects.push_back(new Pit(x,y,this));
            
            
            m_remainingBacteria+=10;
        }
        else {j--;}
    }
    else
        j--;}
    
    for(int j =0; j<(min(5*getLevel(),25));j++){
        double x = rand()%VIEW_WIDTH;
        double y = rand()%VIEW_HEIGHT;
        double product = pow((x-VIEW_WIDTH/2),2)+pow(y-VIEW_HEIGHT/2,2);
        
        if(sqrt(product)<=120){
            list<Actor*>::iterator it1 = objects.begin();
            bool flag = true;
            for(;it1!=objects.end();it1++){
                if(isObjectsOverlapped(x, y, (*it1)->getX(), (*it1)->getY()))
                    flag = false;}
            if(flag){objects.push_back(new Food(x,y,this));}
            else j--;
        }
        else
            j--;
    }
    
    for(int j =0; j<(max(180-20*getLevel(),20));j++){
        double x = rand()%VIEW_WIDTH;
        double y = rand()%VIEW_HEIGHT;//how to get x,y coordinate
        double product = pow((x-VIEW_WIDTH/2),2)+pow(y-VIEW_HEIGHT/2,2);
        if(sqrt(product)<=120){
            list<Actor*>::iterator it1 = objects.begin();
            bool flag = true;
            for(;it1!=objects.end();it1++){
                if(!(*it1)->canBlock()){
                    if(isObjectsOverlapped(x, y, (*it1)->getX(), (*it1)->getY()))
                        flag = false;
                }
            }
            if(flag){objects.push_back(new Dirt(x,y,this));}
            else j--;
        }
        else
            j--;
    }
    
    return GWSTATUS_CONTINUE_GAME;}

int StudentWorld::move()
{
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    if(player->isAlive()){
        player->doSomething();
        if(!player->isAlive())
            return GWSTATUS_PLAYER_DIED;
        if(m_remainingBacteria==0){
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;}
    }
    list<Actor*>::iterator it = objects.begin();
    for(;it!=objects.end(); it++){
        if((*it)->isAlive()){
            (*it)->doSomething();
            if(!player->isAlive())
                return GWSTATUS_PLAYER_DIED;
            if(m_remainingBacteria==0)
                return GWSTATUS_FINISHED_LEVEL;
        }}
    
    removeDeadObjects();
    addNewActor();
    updateDisplayText();
    
    return GWSTATUS_CONTINUE_GAME;}
    
void StudentWorld::cleanUp()
{
    list<Actor*>::iterator it = objects.begin();
    for (; it!= objects.end(); ){
        Actor* a = *it;
        delete a;
        it = objects.erase(it);}
    m_remainingBacteria-=10;
    delete player;
    player = nullptr;
    
}

void StudentWorld::removeDeadObjects(){
   list<Actor*>::iterator it = objects.begin();
    for(;it!=objects.end();){
        if(!(*it)->isAlive()){
            Actor* a = *it;
            it = objects.erase(it);
            delete a;
        }
        else it++;}
    
}

void StudentWorld:: addNewActor(){
    addFungus();
    addGoodies();
    return;
}

void StudentWorld::updateDisplayText(){
    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
    oss<<setw(5)<<getScore();
    oss <<"  Level: "<<getLevel()<<"  Lives: "<<getLives()<<"  health: "<< player->getPoints()<<"  Sprays: "<<player->getNofSpray()<<"  Flames: "<<player->getNOfFlame()<<" rb:"<<m_remainingBacteria;
    setGameStatText(oss.str());
}

double StudentWorld::DistanceOf2Objects(double x1, double y1, double x2, double y2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

bool StudentWorld::isObjectsOverlapped(double x1,double y1,double x2,double y2){
    int distance=DistanceOf2Objects(x1, y1, x2, y2);
    if(distance<=SPRITE_RADIUS*2)
        return true;
    return false;}

bool StudentWorld:: ProjectileOverlapped(double x, double y,int points){ 
    list<Actor*>::iterator it = objects.begin();
    for(;it!=objects.end();it++){
        if((*it)->damageable()&&(*it)->isAlive()){
            if(isObjectsOverlapped(x, y, (*it)->getX(), (*it)->getY())){
                if((*it)->hasHitpoints()){
                    (*it)->setPoints(points,1);
                    if((*it)->getPoints()<=0){
                        (*it)->setDead();
                        (*it)->playSound(1);
                        increaseScore(100);
                        increaseBac(-1);
                        int i = randInt(1, 2);
                        if(i==1){addGoodies();}
                        return true;
                    }
                    else{
                        (*it)->playSound(2);
                        return true;
                    }
                }
                else{
                    (*it)->setDead();
                    return true;
                }
            }
        }
    }
    return false;
}

bool StudentWorld:: BacteriaOverlapped(double x1, double y1, double x2, double y2){
    int distance=DistanceOf2Objects(x1, y1, x2, y2);
    if(distance<=SPRITE_RADIUS)
        return true;
    return false;
}

bool StudentWorld::isSocratesOverlapped(double x, double y){
    if(isObjectsOverlapped(x, y, player->getX(), player->getY()))
        return true;
    else
        return false;}

bool StudentWorld::isFoodOverlapped(double x, double y){
    list<Actor*>::iterator it = objects.begin();
    for(;it!=objects.end();it++){
        if((*it)->canBeEaten()&&(*it)->isAlive()){
            if(isObjectsOverlapped(x, y, (*it)->getX(), (*it)->getY())){
                (*it)->setDead();
                return true;
            }
        }
    }
    return false;
}

void StudentWorld:: setSocrateHealth(int amount, int dir){
    player->setPoints(amount, dir);
    if(player->getPoints()<=0){
        player->setDead();
        playSound(SOUND_PLAYER_DIE);
    }
    else if(dir==1)
        playSound(SOUND_PLAYER_HURT);
}

void StudentWorld::chargeFlames(){
    player->setNOfFlame(player->getNOfFlame()+5);}

void StudentWorld::addGoodies(){
    int chance = max(510- getLevel() * 10, 250);
    int a = randInt(0, chance-1);
    if(a==0){
        double startX = randInt(0, VIEW_WIDTH-1);
        int chance1 = randInt(0, 1);
        double startY;
        if(chance1==0)
            startY = VIEW_RADIUS+sqrt(pow(VIEW_RADIUS, 2)-pow(abs(VIEW_RADIUS-startX), 2));
        else
            startY = VIEW_RADIUS-sqrt(pow(VIEW_RADIUS, 2)-pow(abs(VIEW_RADIUS-startX), 2));
        int chance2 = randInt(1, 10);
        if(chance2==1||chance2==2||chance2==3||chance2==4||chance2==5||chance2==6){
            objects.push_back(new RestoreHealth(startX,startY,this,max(rand()% (300-10* getLevel()), 50)));
        }
        else if (chance2==7||chance2 ==8||chance2 ==9){
            objects.push_back(new FlameThrower(startX,startY,this,max(rand()% (300-10* getLevel()), 50)));
        }
        else{
            objects.push_back(new ExtraLife(startX,startY,this,max(rand()% (300-10* getLevel()), 50)));
        }}
    }

void StudentWorld::addSpray(){
    double startX = player->getX()+cos(player->getDirection()*PI/180.0)*SPRITE_RADIUS;
    double startY = player->getY()+sin(player->getDirection()*PI/180.0)*SPRITE_RADIUS;
    objects.push_back(new Spray(startX,startY, player->getDirection(), this));}

void StudentWorld::addFlame(){
    int dir = 0;
    for(int i =0;i<16;i++,dir+=22){
        double startX =player->getX()+cos(((player->getDirection())+dir)* PI / 180.0) * SPRITE_RADIUS;
        double startY =player->getY()+sin(((player->getDirection())+dir)* PI / 180.0) * SPRITE_RADIUS;
        objects.push_back(new Flame(startX,startY, (player->getDirection())+dir, this));}}

void StudentWorld::addFungus(){
    int ChanceFungus = max(510-getLevel() * 10, 200);
    int chance = randInt(0, ChanceFungus-1);
    if(chance==0){
        double startX = randInt(0, VIEW_WIDTH-1);
        int chance1 = randInt(0, 1);
        double startY;
        if(chance1==0)
            startY = VIEW_RADIUS+sqrt(pow(VIEW_RADIUS, 2)-pow(abs(VIEW_RADIUS-startX), 2));
        else
            startY = VIEW_RADIUS-sqrt(pow(VIEW_RADIUS, 2)-pow(abs(VIEW_RADIUS-startX), 2));
        objects.push_back(new Fungus(startX,startY,this,max(rand() % (300-10 * getLevel()), 50)));
    }
}



bool StudentWorld::canMoveForward(double x, double y){
    if(DistanceOf2Objects(x, y, VIEW_WIDTH/2, VIEW_HEIGHT/2)>=VIEW_RADIUS){
    return false;}
    
    list<Actor*>::iterator it = objects.begin();
    
    while(it!=objects.end()){
        if((*it)->canBlock()){
            if(BacteriaOverlapped(x, y, (*it)->getX(), (*it)->getY()))
                return false;}
        it++;
    }
    
    
    return true;
}

bool StudentWorld::isFoodFoundin128P(double x, double y){
    list<Actor*>::iterator it = objects.begin();
    while(it!=objects.end()){
        if((*it)->canBeEaten()==true && DistanceOf2Objects(x, y, (*it)->getX(), (*it)->getY())<=128)
            return true;
        it++;
    }
    
    return false;
}


void StudentWorld::changeDirToFood(double x,double y,Actor* bacteria){
    list<Actor*>::iterator it = objects.begin();
    int xOfFood = 0;
    int yOfFood= 0;
    int DirOfFood=0;
    
    while(it!=objects.end()){
        if((*it)->canBeEaten()==true && DistanceOf2Objects(x, y, (*it)->getX(), (*it)->getY())<=128){
            xOfFood = (*it)->getX();
            yOfFood = (*it)->getY();
            DirOfFood = (atan((y-yOfFood)/(x-xOfFood)))*180/PI;
            break;
        }
        it++;
    }
    if(x<xOfFood){
        bacteria->setDirection(DirOfFood);}
    else{bacteria->setDirection(DirOfFood+180);}
}

bool StudentWorld::canMoveForwardDirt(double x, double y, Actor* bacteria,int distance){
    
    if(canMoveForward(x, y))
    {   bacteria->moveForward(distance);
        return true;}
    return false;
}

bool StudentWorld::isSocrateFoundinNP(double x, double y,int z){
    if(DistanceOf2Objects(x, y, player->getX(), player->getY())<=z){
        return true;
    }
    return false;
}

void StudentWorld::changeDirToSocrate(double x, double y, Actor* bacteria){
    double xOfSocrate = player->getX();
    double yOfSocrate = player->getY();
    int DirOfSocrate = (atan((y-yOfSocrate)/(x-xOfSocrate)))*180/PI;
    if(x<xOfSocrate){
        bacteria->setDirection(DirOfSocrate);}
    else
        bacteria->setDirection(DirOfSocrate+180);
}

void StudentWorld::addRSmna(double x, double y){
    objects.push_back(new RSmna(x,y,this));
}

void StudentWorld::addASmna(double x, double y){
    objects.push_back(new ASmna(x,y,this));
}

void StudentWorld::addEC(double x, double y){
    objects.push_back(new EC(x,y,this));}

void StudentWorld::increaseBac(int n){m_remainingBacteria+=n;}
