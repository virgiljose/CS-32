#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include "Field.h"
#include <string>
#include <vector>
#include <list>

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    void updateTickCount();
    int getCurrentTicks();
    void updatePosition(Actor* actor, int oldx, int oldy);
    void resetMove();
    bool canMove(int x, int y);
    void updateDisplayText();
    std::string formatDisplayText(int ticks, int ants0, int ants1, int ants2, int ants3, int winningAntNum);
    bool loadField(int x, int y, bool ah0, bool ah1, bool ah2, bool ah3);
    void decomposeInsect(int x, int y, int foodUnits);
    std::list<Actor*> findActor(int x, int y);
    void removeTheDead(int x, int y);
    
    void setAnts(int numberAnts, int colonyID);
    void setTickAnt(int tick, int colonyID);
    int winningAntNum();
    
    int insectEat(int x, int y, int eatThisMuch);
    bool antCheckPheromone(int x, int y, std::string type);
    bool antCheckInsect(int x, int y, std::string type);
    bool insectCheckObject(int x, int y, std::string type);
    
    void newFood(int x, int y, int amt);
    void newPheromone(int x, int y, std::string image);
    void newAnt(int x, int y, Actor* a);
    void newGrasshopper(int x, int y);
    
    bool antgrasshopperbite(Actor* itself, int x, int y);
    
    virtual void cleanUp();

private:
    std::list<Actor*> m_actors[64][64]; // DATA STRUCTURE OF ALL OBJECTS

    // OTHER HELPFUL MEMBER VARIABLES:
    
    std::vector<Compiler*> m_compilers; // Compilers to be used for ants and anthills
    std::vector<int> tickofant;         // keep track of tick when most recent ant of a given colony was created
    std::vector<int> ants;              // keep track of number of ants from each anthill
    
    int m_ticks;                        // ticks
};

#endif // STUDENTWORLD_H_
