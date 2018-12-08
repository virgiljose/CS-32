#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "Compiler.h"
#include <list>

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, StudentWorld* stdwld, int initenergy, Direction dir, double size, unsigned int depth, std::string type);
    virtual ~Actor();
    virtual void doSomething()=0;
    int getEnergyPoints() const;
    bool isDead() const;
    
    int setHitPts(int set);
    void setDead();
    void setNewMove();
    void setMoved();
    bool movedThisTurn() const;
    
    StudentWorld* callWorld() const;
    
    virtual bool blocksInsect() const;
    
    std::string typeOfActor() const;
    
    int getSleep();
    void setSleep(int howLong);
    virtual void retaliate();
    bool eat(int eatThisMuch);
    
    bool checkBit();
    void setBit(bool bit);
    
private:
    StudentWorld* theworld;
    int energypoints;
    bool m_isDead;
    bool m_moved;
    std::string m_type;
    int m_sleepTimer;
    bool amIBit;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// BASE CLASSES//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

class Insect : public Actor
{
public:
    Insect(int imageID, int startX, int startY, StudentWorld* stdwld, int initenergy, double size, unsigned int depth, std::string type);
    virtual ~Insect();
    virtual bool checkEnergy();
    virtual void decompose(int foodUnits);
    
    bool canIMove(int x, int y) const;
    bool attemptMove();
    
    virtual void checkForHarm();
    
    bool checkBlocked();
    void setBlocked(bool block);
private:
    bool blocked;
};


class EnergyHolder : public Actor
{
public:
    EnergyHolder(int startX, int startY, StudentWorld* stdwld, int imageID, int energyPoints, std::string type);
    virtual ~EnergyHolder();
    virtual void doSomething();
};

class Harmer : public Actor
{
public:
    Harmer(int startX, int startY, StudentWorld* stdwld, int imageID, std::string type);
    virtual ~Harmer();
    virtual void doSomething();
};

class Pebble : public Actor
{
public:
    Pebble(int startX, int startY, StudentWorld* stdwld);
    virtual ~Pebble();
    virtual void doSomething();
    virtual bool blocksInsect() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DERIVED CLASSES///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

class Ant : public Insect
{
public:
    Ant(int startX, int startY, StudentWorld* stdwld, int imageID, std::string type, Compiler* USCSucks);
    virtual ~Ant();
    virtual void doSomething();
    int getColonyID();
    bool runCommand(const Compiler& c);
    bool checkCondition(int operand);
private:
    Compiler* m_compiler;
    int colonyID;
    int foodStored;
    int instructionCounter;
    std::string m_pheromone;
    int m_lastrandomnumber;
    int ic;
};

class Grasshopper : public Insect
{
public:
    Grasshopper(int startX, int startY, StudentWorld* stdwld, int imageID, std::string type);
    virtual ~Grasshopper();
    virtual void doSomething();
    
    double getDistance();
    void setNewDistance();
    void decrementDist();
    virtual bool checkEvolve(); 
    virtual bool bite();
    virtual bool jump();
    virtual void retaliate();
private:
    double m_distance;
};

class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(int startX, int startY, StudentWorld* stdwld);
    virtual ~BabyGrasshopper();
    virtual void checkForHarm();
    virtual bool checkEvolve();
    virtual bool bite();
    virtual bool jump();
};

class Pheromone : public EnergyHolder
{
public:
    Pheromone(int startX, int startY, StudentWorld* stdwld, int imageID, std::string type);
    virtual ~Pheromone();
    virtual void doSomething();
};

class Food : public EnergyHolder
{
public:
    Food(int startX, int startY, StudentWorld* stdwld, int foodUnits);
    virtual ~Food();
};

class AntHill : public EnergyHolder
{
public:
    AntHill(int startX, int startY, StudentWorld* stdwld, std::string type, Compiler* USCSucks);
    virtual ~AntHill();
    virtual void doSomething();
    int getNumberAnts();
private:
    int m_colony;
    int numberAnts;
    int antID;
    int tickofnewant;
    Compiler* m_antlogic;
};

class WaterPool : public Harmer
{
public:
    WaterPool(int startX, int startY, StudentWorld* stdwld);
    virtual ~WaterPool();
};

class Poison : public Harmer
{
public:
    virtual ~Poison();
    Poison(int startX, int startY, StudentWorld* stdwld);
};


#endif // ACTOR_H_
