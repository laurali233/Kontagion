#include "Actor.h"
#include "StudentWorld.h"
// projectile cannot display
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//Actor implementation ------------------------------------------------
Actor::Actor(int imageID, double startX, double startY, Direction dir , int depth, double size, StudentWorld* sw): GraphObject(imageID, startX, startY,dir,depth,size){
    m_isAlive = true;
    m_sw = sw;
}

Actor::~Actor(){}

bool Actor::isAlive(){return m_isAlive;}
StudentWorld* Actor::getSW(){return m_sw;}
void Actor::setDead(){m_isAlive= false;}
bool Actor::hasHitpoints(){return false;}
bool Actor::setPoints(int points, int addOrDe){return false;}
int Actor::getPoints(){return 0;}

//StationaryObjects implementation ------------------------------------

StationaryObjects::StationaryObjects(int imageID, double startX, double startY, Direction dir , int depth, double size, StudentWorld* sw):Actor(imageID, startX, startY, dir, depth, size, sw){};

void StationaryObjects::playSound(int HurtOrDie){return;}
bool StationaryObjects::canBeEaten(){return false;}
bool StationaryObjects::canBlock(){return false;}


//Dirt implementation
Dirt::Dirt(double startX, double startY,StudentWorld* sw): StationaryObjects(IID_DIRT, startX, startY, 0, 1, 1.0, sw){};
void Dirt::doSomething(){return;}
bool Dirt::damageable(){return true;}
bool Dirt::canBlock(){return true;}

//Pit implementation ---------------------------------------

Pit::Pit(double startX, double startY,StudentWorld* sw):StationaryObjects(IID_PIT, startX, startY, 0, 1, 1.0, sw){
    m_nOfAS = 3;
    m_nOfEC = 2;
    m_nOfRS = 5;
    m_BacteriaLeft = 10;
}
bool Pit::damageable(){return false;}
void Pit::pickABacteriaToGenerate(){
    int i =1;
    while(i==1){
        int a = randInt(1, 3);
        if(a==1&&m_nOfRS>0){
            getSW()->addRSmna(getX(), getY());
            m_nOfRS--;
            m_BacteriaLeft--;
            i++;
        }
        else if(a==2&&m_nOfAS>0){
            getSW()->addASmna(getX(), getY());
            m_nOfAS--;
            m_BacteriaLeft--;
            i++;
        }
        else if(a==3&&m_nOfEC>0){
            getSW()->addEC(getX(), getY());
            m_nOfEC--;
            m_BacteriaLeft--;
            i++;
        }
        
    }
    
}

void Pit::doSomething(){
    
    if(m_BacteriaLeft<=0){
        setDead();
        return;}
    else{
        int a =randInt(1, 50);
        if(a==1){pickABacteriaToGenerate();
            getSW()->playSound(SOUND_BACTERIUM_BORN);
        }
    }
}

//Food implementation --------------------------------------------------
Food::Food(double startX,double startY,StudentWorld* sw):StationaryObjects(IID_FOOD, startX, startY, 90, 1, 1.0, sw){};
void Food::doSomething(){return;}
bool Food::damageable(){return false;}
bool Food::canBeEaten(){return true;}

//MayGoesAway implementation----------------------------

MayGoesAway::MayGoesAway(int imageID, double startX, double startY,StudentWorld* sw, int LifeTime):StationaryObjects(imageID, startX, startY, 0, 1, 1.0, sw){
    m_lifeTime = LifeTime;
    }
bool MayGoesAway::damageable(){
    return true;}
int MayGoesAway::GetLifeTime(){
    return m_lifeTime;}
void MayGoesAway::DeLifeTime(){
    m_lifeTime--;
}


//RestoreHealth implementation ---------------------------------------
RestoreHealth::RestoreHealth(double startX, double startY,StudentWorld* sw,int LifeTime):MayGoesAway(IID_RESTORE_HEALTH_GOODIE,startX,startY,sw,LifeTime){}

void RestoreHealth::doSomething(){
    if(!isAlive())
        return;
    if(getSW()->isSocratesOverlapped(getX(), getY())){
        getSW()->increaseScore(250);
        setDead();
        getSW()->playSound(SOUND_GOT_GOODIE);
        getSW()->setSocrateHealth(100, 3);
        return;
    }
    if(GetLifeTime()<=0){
        setDead();
        return;}
    DeLifeTime();
}

//FlameThrower implementation ---------------------------------------
FlameThrower::FlameThrower(double startX, double startY,StudentWorld* sw,int LifeTime):MayGoesAway(IID_FLAME_THROWER_GOODIE,startX,startY,sw,LifeTime){}

void FlameThrower::doSomething(){
    if(!isAlive())
        return;
    if(getSW()->isSocratesOverlapped(getX(), getY())){
        getSW()->increaseScore(300);
        setDead();
        getSW()->playSound(SOUND_GOT_GOODIE);
        getSW()->chargeFlames();
        return;
    }
    if(GetLifeTime()<=0){
        setDead();
        return;}
    DeLifeTime();
}

//ExtraLife implementation ------------------------------------------
ExtraLife::ExtraLife(double startX, double startY,StudentWorld* sw, int LifeTime):MayGoesAway(IID_EXTRA_LIFE_GOODIE, startX, startY, sw, LifeTime){}

void ExtraLife::doSomething(){
    if(!isAlive())
        return;
    if(getSW()->isSocratesOverlapped(getX(), getY())){
        getSW()->increaseScore(500);
        setDead();
        getSW()->incLives();
        return;
    }
    if(GetLifeTime()<=0){
        setDead();
        return;}
    DeLifeTime();
}

//Fungus implementation --------------------------------------------

Fungus::Fungus(double startX, double startY,StudentWorld* sw, int LifeTime):MayGoesAway(IID_FUNGUS, startX, startY, sw, LifeTime){}

void Fungus::doSomething(){
    if(!isAlive())
        return;
    if(getSW()->isSocratesOverlapped(getX(), getY())){
        getSW()->increaseScore(-50);
        setDead();
        getSW()->setSocrateHealth(20, 1);
        return;}
    if(GetLifeTime()<=0){
        setDead();
        return;}
    DeLifeTime();}


//MovableObjects implementation-----------------------------------
MovableObjects::MovableObjects(int imageID, double startX, double startY, int points, Direction dir , int depth, double size, StudentWorld* sw):Actor(imageID, startX, startY, dir, depth, size, sw){m_hitPoints=points;}
int MovableObjects:: getPoints(){
    return m_hitPoints;}
bool MovableObjects::hasHitpoints(){
    return true;}
bool MovableObjects::canBeEaten(){return false;}
bool MovableObjects::setPoints(int points,int addOrDe){
    if(addOrDe==1){
        m_hitPoints-=points;
    }
    else if(addOrDe==2)
        m_hitPoints+=points;
    else
        m_hitPoints = points;
    return true;}
bool MovableObjects::canBlock(){return false;}

//Bacteria implementation -----------------------------------------
Bacteria::Bacteria(int imageID, double startX, double startY, int points,int damage,StudentWorld* sw):MovableObjects(imageID, startX, startY, points,90,0, 1.0, sw){
    m_planDistance =0;
    m_foodEaten = 0;
    m_damage =damage;
}
void Bacteria::doSomething(){
    if(!isAlive())
        return;
    
    bool flag = goForSocrate();
    
    if(getSW()->isSocratesOverlapped(getX(), getY())){
        getSW()->setSocrateHealth(getDamage(), 1);
    }
    else if(m_foodEaten==3){
        double newX;
        if(getX()<(VIEW_WIDTH/2))
            newX = getX()+SPRITE_RADIUS;
        else
            newX = getX()-SPRITE_RADIUS;
        
        double newY;
        if(getY()<(VIEW_HEIGHT/2))
            newY = getY()+SPRITE_RADIUS;
        else
            newY = getY()-SPRITE_RADIUS;
        if(getDamage()==1)
            getSW()->addRSmna(newX, newY);
        else if (getDamage()==2)
            getSW()->addASmna(newX, newY);
        else if (getDamage()==4)
            getSW()->addEC(newX, newY);
        getSW()->increaseBac(1);
        CleanEatenBacteria();}
    else if(getSW()->isFoodOverlapped(getX(), getY())){
        m_foodEaten+=1;}
    if(flag)
        return;
    MoveWhenDisIsGreaterThan0();
    return;}

void Bacteria::MoveWhenDisIsGreaterThan0(){
    if(getPlanDistance()>0){
        deDistance();
        double newX = getX()+cos(getDirection()*PI/180)*3;
        double newY = getY()+sin(getDirection()*PI/180)*3;
        if(getSW()->canMoveForward(newX,newY))
            moveTo(newX,newY);
        else{
            int newDir =randInt(0, 359);//和下面的重新找方向可以写一个function
            setDirection(newDir);
            resetDistance();}
        return;
    }
    else{
        if(!getSW()->isFoodFoundin128P(getX(), getY())){
            int newDir =randInt(0, 359);
            setDirection(newDir);
            resetDistance();
            return;
        }
        else{
            getSW()->changeDirToFood(getX(), getY(), this);
            double newX = getX()+cos(getDirection()*PI/180)*3;
            double newY = getY()+sin(getDirection()*PI/180)*3;
            if(!(getSW()->canMoveForwardDirt(newX, newY, this,3)))
            {   int newDir =randInt(0, 359);
                setDirection(newDir);
                resetDistance();
                return;}
            }
    }
}

int Bacteria::getPlanDistance(){return m_planDistance;}
bool Bacteria::damageable(){return true;}
void Bacteria::CleanEatenBacteria(){m_foodEaten=0;}
void Bacteria::deDistance(){m_planDistance--;}
void Bacteria:: resetDistance(){m_planDistance=10;}
int Bacteria::getDamage(){return m_damage;}


//RSmna implementation --------------------------------------------
RSmna::RSmna(double startX, double startY,StudentWorld* sw):Bacteria(IID_SALMONELLA, startX, startY, 4, 1,sw){}
void RSmna::playSound(int HurtOrDie){
    if(HurtOrDie==1)
        getSW()->playSound(SOUND_SALMONELLA_DIE);
    if(HurtOrDie==2)
        getSW()->playSound(SOUND_SALMONELLA_HURT);
}
bool RSmna::goForSocrate(){return false;}

//RSmna implementation --------------------------------------------
ASmna::ASmna(double startX, double startY, StudentWorld* sw):Bacteria(IID_SALMONELLA, startX, startY, 10,2, sw){}
void ASmna::playSound(int HurtOrDie){
    if(HurtOrDie==1)
        getSW()->playSound(SOUND_SALMONELLA_DIE);
    if(HurtOrDie==2)
        getSW()->playSound(SOUND_SALMONELLA_HURT);
    }
bool ASmna::goForSocrate(){
    if(getSW()->isSocrateFoundinNP(getX(), getY(),72)){
        getSW()->changeDirToSocrate(getX(), getY(), this);
        double newX = getX()+cos(getDirection()*PI/180)*3;
        double newY = getY()+sin(getDirection()*PI/180)*3;
        getSW()->canMoveForwardDirt(newX,newY, this,3);
        return true;
    }
    return false;
}

//EC implementation-------------------------------------------------
EC::EC(double startX, double startY, StudentWorld* sw):Bacteria(IID_ECOLI, startX, startY, 5, 4, sw){}
    
void EC::playSound(int HurtOrDie){
    if(HurtOrDie==1)
        getSW()->playSound(SOUND_ECOLI_DIE);
    if(HurtOrDie==2)
        getSW()->playSound(SOUND_ECOLI_HURT);}
bool EC::goForSocrate(){return false;}
void EC::MoveWhenDisIsGreaterThan0(){
    if(getSW()->isSocrateFoundinNP(getX(), getY(), 256)){
        for(int i = 0;i<10;i++){
            getSW()->changeDirToSocrate(getX(), getY(), this);
            double newX = getX()+cos(getDirection()*PI/180)*2;
            double newY = getY()+sin(getDirection()*PI/180)*2;
            if(getSW()->canMoveForwardDirt(newX, newY, this,2))
                return;
            else
                setDirection(getDirection()+10);}
    }}


//Socrates implementation -----------------------------------------
Socrates::Socrates(StudentWorld* sw):MovableObjects(IID_PLAYER, 0, 128, 100, 0, 0, 1.0, sw){
    m_nOfFlame = 200;
    m_nOfSpray = 20;
    m_positionAngel = 180;
}

int Socrates::getAngel(){return m_positionAngel;}
int Socrates::getNOfFlame(){return m_nOfFlame;}
void Socrates::playSound(int HurtOrDie){return;}
int Socrates::getNofSpray(){return m_nOfSpray;}
bool Socrates::damageable(){return false;}
void Socrates::setNOfFlame(int n){m_nOfFlame = n;}
void Socrates::doSomething(){
    if(!isAlive())
        return;
    int input;
    if(getSW()->getKey(input))
    {   double x,y;
        switch (input)
        {
            case KEY_PRESS_SPACE:
                if(m_nOfSpray>0){
                    getSW()->addSpray();
                    getSW()->playSound(SOUND_PLAYER_SPRAY);
                    m_nOfSpray--;}
                break;
            case KEY_PRESS_ENTER:
                if(m_nOfFlame>0){
                    getSW()->addFlame();
                    getSW()->playSound(SOUND_PLAYER_FIRE);
                    m_nOfFlame--;}
                break;
            case KEY_PRESS_LEFT:
                m_positionAngel-=5;
                if(m_positionAngel<0){
                    m_positionAngel+=360;}
                y= (sin(m_positionAngel*PI/180)+1)*VIEW_RADIUS;
                x= (1+cos(m_positionAngel*PI/180))*VIEW_RADIUS;
                moveTo(x, y);
                setDirection(180+m_positionAngel);
                break;
            case KEY_PRESS_RIGHT:
                m_positionAngel+=5;
                if(m_positionAngel>360){
                m_positionAngel-=360;}
                y=(sin(m_positionAngel*PI/180)+1)*VIEW_RADIUS;
                x= (1+cos(m_positionAngel*PI/180))*VIEW_RADIUS;
                moveTo(x, y);
                setDirection(180+m_positionAngel);
                break;
        }
    }
    else{
        if(m_nOfSpray<20)
            m_nOfSpray++;}
    return;
}

// Projectile implementation

Projectile::Projectile(int imageID, double startX, double startY, Direction dir , int depth, double size, StudentWorld* sw,int traveldistance,int points):Actor(imageID, startX, startY, dir, depth, size, sw){
    m_travelDistance = traveldistance;}
bool Projectile::damageable(){return false;}
int Projectile::getTravelDistance(){return m_travelDistance;}
void Projectile::DeTravelDistance(int num){m_travelDistance-=num;}
bool Projectile::canBlock(){return false;}
bool Projectile::canBeEaten(){return false;}
void Projectile::playSound(int HurtOrDie){return;}
void Projectile::doSomething(){
        if(!isAlive())
            return;
        if(getSW()->ProjectileOverlapped(getX(), getY(),points)){
            setDead();
            return;}
        else{   moveForward(SPRITE_RADIUS*2);
            DeTravelDistance(SPRITE_RADIUS*2);
        }
        if(getTravelDistance()<=0)
            setDead();
        return;
}

//Flame implementation ------------------------------------------------
Flame::Flame(double startX, double startY, Direction dir,StudentWorld* sw):Projectile(IID_FLAME , startX, startY, dir, 1, 1.0, sw, 32,5){}

//Spray implementation ------------------------------------------------
Spray::Spray(double startX, double startY, Direction dir,StudentWorld* sw):Projectile(IID_SPRAY, startX, startY, dir, 1, 1.0, sw, 112,2){}

