#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ACTOR (BASE CLASS) IMPLEMENTATION ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Actor::Actor(int imageID, int startX, int startY, StudentWorld* stdwld, int initenergy, Direction dir, double size, unsigned int depth, std::string type)
    : GraphObject(imageID, startX, startY, dir, depth, size)
    {
        energypoints = initenergy;
        m_isDead = false;
        theworld = stdwld;
        m_moved = false;
        m_type = type;
        m_sleepTimer = 0;
    }
Actor::~Actor()
{}

int Actor::getEnergyPoints() const {
    return energypoints;
}


bool Actor::isDead() const {
    return m_isDead;
}

int Actor::setHitPts(int set)
{
    int initialEnergy = getEnergyPoints();
    energypoints += set;
    
    if(getEnergyPoints() <= 0)
        setDead();
    
    if(getEnergyPoints() <= 0)
        return -initialEnergy;
    else
        return set;
}

void Actor::setDead() {
    m_isDead = true;
}

void Actor::setNewMove() {
    m_moved = false;
}

void Actor::setMoved() {
    m_moved = true;
}

bool Actor::movedThisTurn() const {
    return m_moved;
}

StudentWorld* Actor::callWorld() const {
    return theworld;
}

bool Actor::blocksInsect() const {
    return false;
}

std::string Actor::typeOfActor() const {
    return m_type;
}

int Actor::getSleep() {
    return m_sleepTimer;
}

void Actor::setSleep(int howLong) {
    m_sleepTimer += howLong;
}

void Actor::retaliate()
{
    return;
}

bool Actor::eat(int eatThisMuch) // because anthills "eat" too
{
    int hasEaten = -callWorld()->insectEat(getX(), getY(), eatThisMuch);
    if(hasEaten)
    {
        setHitPts(hasEaten);
        if(randInt(0, 1))
            return true;
        else
            return false;
    }
    else
        return false;
}

void Actor::setBit(bool bit)
{
    amIBit = bit;
}

bool Actor::checkBit()
{
    return amIBit;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INSECT (BASE CLASS) IMPLEMENTATION ///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Insect::Insect(int imageID, int startX, int startY, StudentWorld* stdwld, int initenergy, double size, unsigned int depth, std::string type)
: Actor(imageID, startX, startY, stdwld, initenergy, static_cast<GraphObject::Direction>(rand() % 4 + 1), size, depth, type)
{}

Insect::~Insect()
{}


bool Insect::checkEnergy()
{
    if(getEnergyPoints() <= 0)
    {
        decompose(100);
        setDead();
        return true;
    }
    return false;
}

void Insect::decompose(int foodUnits) {
    callWorld()->decomposeInsect(getX(), getY(), foodUnits);
}


bool Insect::canIMove(int x, int y) const
{
    return callWorld()->canMove(x,y);
}

bool Insect::attemptMove()
{
    switch(getDirection())
    {
        case right:
        {
            if(canIMove(getX() + 1, getY()))
            {
                setDirection(right);
                moveTo(getX()+1, getY());
                return true;
                break;
            }
        }
        case down:
        {
            if(canIMove(getX(), getY()-1))
            {
                setDirection(down);
                moveTo(getX(), getY()-1);
                return true;
                break;
            }
        }
        case up:
        {
            if(canIMove(getX(), getY()+1))
            {
                setDirection(up);
                moveTo(getX(), getY()+1);
                return true;
                break;
            }
        }
        case left:
        {
            if(canIMove(getX() - 1, getY()))
            {
                setDirection(left);
                moveTo(getX()-1, getY());
                return true;
                break;
            }
        }
        default:
            setBlocked(true);
            return false;
    }
}


void Insect::checkForHarm()
{
    if(callWorld()->insectCheckObject(getX(), getY(), "water pool"))
        setSleep(0);
    if(callWorld()->insectCheckObject(getX(), getY(), "poison"))
        setHitPts(0);
}

bool Insect::checkBlocked()
{
    return blocked;
}

void Insect::setBlocked(bool block)
{
    blocked = block;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ENERGYHOLDER (BASE CLASS) IMPLEMENTATION /////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EnergyHolder::EnergyHolder(int startX, int startY, StudentWorld* stdwld, int imageID, int energyPoints, std::string type)
: Actor(imageID, startX, startY, stdwld, energyPoints, right, 0.25, 2, type)
{}

EnergyHolder::~EnergyHolder()
{}

void EnergyHolder::doSomething()
{
    if(getEnergyPoints() <= 0)
        setDead();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HARMER (BASE CLASS) IMPLEMENTATION////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Harmer::Harmer(int startX, int startY, StudentWorld* stdwld, int imageID, std::string type)
: Actor(imageID, startX, startY, stdwld, 1, right, 0.25, 2, type)
{}

Harmer::~Harmer()
{}

void Harmer::doSomething()
{
    setMoved();
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ANT IMPLEMENTATION//////////////////////////////////////////////////////???????///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Ant::Ant(int startX, int startY, StudentWorld* stdwld, int imageID, std::string type, Compiler* USCSucks)
 : Insect(imageID, startX, startY, stdwld, 1500, 0.25, 1, type)
{
    m_compiler = USCSucks;
    
    if(imageID == IID_ANT_TYPE0)
        colonyID = 0, m_pheromone = "pheromone 0";
    else if(imageID == IID_ANT_TYPE1)
        colonyID = 1, m_pheromone = "pheromone 1";
    else if(imageID == IID_ANT_TYPE2)
        colonyID = 2, m_pheromone = "pheromone 2";
    else if(imageID == IID_ANT_TYPE3)
        colonyID = 3, m_pheromone = "pheromone 3";
    else
        colonyID = -1; // MEANS THERE IS AN ERROR
    
    foodStored = 0;
    instructionCounter = 0;
    ic = 0;
}

Ant::~Ant()
{}

void Ant::doSomething()
{
    setHitPts(-1);
    
    std::cerr << getEnergyPoints() << std::endl;
    
    setMoved();
    if(checkEnergy())
        return;
    
    if(getSleep() > 0)
        
    {
        setSleep(-1);
        return;
    }
    
        std::cerr << foodStored << std::endl;
    
    if(!runCommand(*m_compiler))
    {
        setDead();
        return;
    }
    setBlocked(false);
    return;
    
}

int Ant::getColonyID()
{
    return colonyID;
}


bool Ant::runCommand(const Compiler& c)
{
    Compiler::Command cmd;
    
    for(int rc = 0; rc < 10;)
    {
        if ( ! c.getCommand(ic, cmd) )
            return false; // error - no such instruction!
    
        if (cmd.opcode == Compiler::moveForward)
        {
            attemptMove();
            ++ic;
            ++rc;
            return true;
        }
        if (cmd.opcode == Compiler::rotateClockwise)
        {
            switch(getDirection())
            {
                case up:
                    setDirection(right);
                    break;
                case right:
                    setDirection(down);
                    break;
                case down:
                    setDirection(left);
                    break;
                case left:
                    setDirection(up);
                    break;
                default:
                    break;
            }
            ++ic;
            ++rc;
            return true;
        }
        if (cmd.opcode == Compiler::rotateCounterClockwise)
        {
            switch(getDirection())
            {
                case up:
                    setDirection(left);
                    break;
                case right:
                    setDirection(up);
                    break;
                case down:
                    setDirection(right);
                    break;
                case left:
                    setDirection(down);
                    break;
                default:
                    break;
            }
            ++ic;
            ++rc;
            return true;
        }
        if (cmd.opcode == Compiler::faceRandomDirection)
        {
            setDirection(static_cast<GraphObject::Direction>(rand() % 4 + 1));
            ++ic;
            ++rc;
            return true;
        }
        if (cmd.opcode == Compiler::bite)
        {
            callWorld()->antgrasshopperbite(this, getX(), getY());
            ++ic;
            ++rc;
            return true;
        }
        if (cmd.opcode == Compiler::emitPheromone)
        {
            if(colonyID != -1)
                callWorld()->newPheromone(getX(), getY(), m_pheromone);
            ++ic;
            ++rc;
            return true;
        }
        if (cmd.opcode == Compiler::pickupFood)
        {
            foodStored += -callWorld()->insectEat(getX(), getY(), 400);
            ++ic;
            ++rc;
            return true;
        }
        if (cmd.opcode == Compiler::dropFood)
        {
            callWorld()->newFood(getX(), getY(), foodStored);
            foodStored = 0;
            ++ic;
            ++rc;
            return true;
        }
        if (cmd.opcode == Compiler::eatFood)
        {
            if(foodStored < 100)
            {
                setHitPts(foodStored);
                foodStored = 0;
            }
            else
            {
                setHitPts(100);
                foodStored -= 100;
            }
            ++ic;
            ++rc;
            return true;
        }
        if (cmd.opcode == Compiler::goto_command)
        {
            ++rc;
            ic = stoi(cmd.operand1);
            continue;
        }
        if (cmd.opcode == Compiler::generateRandomNumber)
        {
            m_lastrandomnumber = randInt(0, stoi(cmd.operand1));
            ++ic;
            ++rc;
            continue;
        }
        if (cmd.opcode == Compiler::if_command)
        {
            if (checkCondition(stoi(cmd.operand1)))
            {
                ++rc;
                ic = stoi(cmd.operand2);
                continue;
            }
            else
            {
                ++ic;
                ++rc;
                continue;
            }
        }
    }
    return true;
}

bool Ant::checkCondition(int operand)
{
    int dirx = getX(), diry = getY();
    if(getDirection() == up)
        diry++;
    else if(getDirection() == down)
        diry--;
    else if(getDirection() == left)
        dirx--;
    else if(getDirection() == right)
        dirx++;
    
    switch(operand)
    {
        case 0:
        {
            return  callWorld()->insectCheckObject(dirx, diry, "poison") ||
                    callWorld()->insectCheckObject(dirx, diry, "water pool") ||
                    callWorld()->antCheckInsect(dirx, diry, typeOfActor());
            break;
        }
        case 1:
        {
            return callWorld()->antCheckPheromone(dirx, diry, typeOfActor());
            break;
        }
        case 2:
        {
            return checkBit();
            break;
        }
        case 3:
        {
            if(foodStored)
                return true;
            else
                return false;
            break;
        }
        case 4:
        {
            return (getEnergyPoints() <= 25);
            break;
        }
        case 5: // i_am_standing_on_my_anthill
        {
            if(colonyID == 0)
                return callWorld()->insectCheckObject(getX(), getY(), "anthill 0");
            if(colonyID == 1)
                return callWorld()->insectCheckObject(getX(), getY(), "anthill 1");
            if(colonyID == 2)
                return callWorld()->insectCheckObject(getX(), getY(), "anthill 2");
            if(colonyID == 3)
                return callWorld()->insectCheckObject(getX(), getY(), "anthill 3");
            return false;
            break;
        }
        case 6: // i_am_standing_on_food
        {
            return callWorld()->insectCheckObject(getX(), getY(), "food");
            break;
        }
        case 7: // i_am_standing_with_an_enemy
        {
            return callWorld()->antCheckInsect(getX(), getY(), typeOfActor());
            break;
        }
        case 8: // i_was_blocked_from_moving
        {
            return checkBlocked();
        }
        case 9: // last_random_number_was_zero
        {
            return m_lastrandomnumber == 0;
            break;
        }
        default:
            break;
    }
    return false; // TEMP
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GRASSHOPPER IMPLEMENTATION//////////////////////////////////////////////???????///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Grasshopper::Grasshopper(int startX, int startY, StudentWorld* stdwld, int imageID = IID_ADULT_GRASSHOPPER, std::string type = "grasshopper")
 : Insect(imageID, startX, startY, stdwld, 500, 0.25, 1, type)
 {
    m_distance = randInt(2, 10);
 }

Grasshopper::~Grasshopper()
{}

void Grasshopper::doSomething()
{
    setHitPts(-1);
    setMoved();
    
    if(getEnergyPoints() <= 0)
    {
        decompose(100);
        setDead();
        return;
    }
    if(getSleep() > 0)
    {
        setSleep(-1);
        return;
    }
    if(checkEvolve())
        return;
    if(bite())
        return;
    if(jump())
        return;
    if(eat(200))
        return;
    
    setNewDistance(); // only does so if neccesary
    
    
    if(attemptMove())
    {
        decrementDist();
        checkForHarm();
    }
    else
        setDirection(static_cast<GraphObject::Direction>(rand() % 4 + 1));
    
    setSleep(2);
    setBlocked(false);
    return;
}

double Grasshopper::getDistance()
{
    return m_distance;
}

void Grasshopper::setNewDistance()
{
    if(getDistance() == 0)
    {
        setDirection(static_cast<GraphObject::Direction>(rand() % 4 + 1));
        m_distance = randInt(2, 10);
    }
}

void Grasshopper::decrementDist()
{
    m_distance--;
}

bool Grasshopper::bite()
 {
     if(!randInt(0, 2))
         return callWorld()->antgrasshopperbite(this, getX(), getY());
     else
         return false;
 }


 bool Grasshopper::jump()
 {
    if(!randInt(0,9))
        {
            const int pie = 4 * atan(1.0);
            
            bool canIJump = false;
            for(int i = 1; i <= 10; i++)
                for(int checkangle = 0; checkangle < 360; checkangle++)
                {
                    if(canIMove(getX() + i*cos(checkangle*pie/180), getY() + i*sin(checkangle*pie/180)))
                        canIJump = true;
                }
            
            if(canIJump)
            {
                bool jumped = false;
                while(!jumped)
                {
                    int jumpTo = randInt(1,10);
                    int angle = randInt(0,359);
                    if(canIMove(getX() + jumpTo*cos(angle*pie/180), getY() + jumpTo*sin(angle*pie/180)))
                    {
                        moveTo(getX() + jumpTo*cos(angle*pie/180), getY() + jumpTo*sin(angle*pie/180));
                        jumped = true;
                        return jumped;
                    }
                }
            }
            else
                return false;
        }
    return false;
 }

bool Grasshopper::checkEvolve()
{
    return false;
}

void Grasshopper::retaliate()
{
    if(randInt(0,1))
        callWorld()->antgrasshopperbite(this, getX(), getY());
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BABYGRASSHOPPER IMPLEMENTATION ///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BabyGrasshopper::BabyGrasshopper(int startX, int startY, StudentWorld* stdwld)
 : Grasshopper(startX, startY, stdwld, IID_BABY_GRASSHOPPER, "baby grasshopper")
{}

BabyGrasshopper::~BabyGrasshopper()
{}

bool BabyGrasshopper::checkEvolve()
{
    if(getEnergyPoints() >= 1600)
    {
        callWorld()->newGrasshopper(getX(), getY());
        decompose(100);
        setDead();
        return true;
    }
    return false;
}

void BabyGrasshopper::checkForHarm()
{
    if(callWorld()->insectCheckObject(getX(), getY(), "water pool"))
        setSleep(2);
    if(callWorld()->insectCheckObject(getX(), getY(), "poison"))
        setHitPts(-150);
}

bool BabyGrasshopper::bite()
{
    return false;
}

bool BabyGrasshopper::jump()
{
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PEBBLE IMPLEMENTATION ////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pebble::Pebble(int startX, int startY, StudentWorld* stdwld)
    : Actor(IID_ROCK, startX, startY, stdwld, 1, right, 0.25, 1, "pebble")
{}

Pebble::~Pebble()
{}

void Pebble::doSomething()
{
    setMoved();
    return;
}

bool Pebble::blocksInsect() const
{
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PHEROMONE IMPLEMENTATION /////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Pheromone::Pheromone(int startX, int startY, StudentWorld* stdwld, int imageID, std::string type)
: EnergyHolder(startX, startY, stdwld, imageID, 256, type)
{}

Pheromone::~Pheromone()
{}

void Pheromone::doSomething()
{
    setHitPts(-1);
    setMoved();
    if(getEnergyPoints() <= 0)
        setDead();
    return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FOOD IMPLEMENTATION //////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Food::Food(int startX, int startY, StudentWorld* stdwld, int foodUnits = 6000)
 : EnergyHolder(startX, startY, stdwld, IID_FOOD, foodUnits, "food")
{}

Food::~Food()
{}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ANTHILL IMPLEMENTATION ////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AntHill::AntHill(int startX, int startY, StudentWorld* stdwld, std::string type, Compiler* USCSucks)
: EnergyHolder(startX, startY, stdwld, IID_ANT_HILL, 8999, type)
{
    if(type == "anthill 0")
        m_colony = 0;
    if(type == "anthill 1")
        m_colony = 1;
    if(type == "anthill 2")
        m_colony = 2;
    if(type == "anthill 3")
        m_colony = 3;
    
    if(m_colony == 0)
        antID = IID_ANT_TYPE0;
    if(m_colony == 1)
        antID = IID_ANT_TYPE1;
    if(m_colony == 2)
        antID = IID_ANT_TYPE2;
    if(m_colony == 3)
        antID = IID_ANT_TYPE3;
    
    m_antlogic = USCSucks;
    numberAnts = 0;
    tickofnewant = 0;
}

AntHill::~AntHill()
{}

void AntHill::doSomething()
{
    setHitPts(-1);
    setMoved();
    
    if(getEnergyPoints() <= 0)
    {
        setDead();
        return;
    }
    
    if(eat(10000))
    {
        return;
    }
    if(getEnergyPoints() >= 2000)
    {
        std::string type = "ant";
        
        if(m_colony == 0)
            type = "ant 0";
        if(m_colony == 1)
            type = "ant 1";
        if(m_colony == 2)
            type = "ant 2";
        if(m_colony == 3)
            type = "ant 3";
        
        Actor* ant = new Ant(getX(), getY(), callWorld(), antID, type, m_antlogic);
        callWorld()->newAnt(getX(), getY(), ant);
        numberAnts++;
        callWorld()->setAnts(numberAnts, m_colony);
        tickofnewant = callWorld()->getCurrentTicks();
        callWorld()->setTickAnt(tickofnewant, m_colony);
        
        setHitPts(-1500);
    }
    return;
}

int AntHill::getNumberAnts()
{
    return numberAnts;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// WATERPOOL IMPLEMENTATION //////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WaterPool::WaterPool(int startX, int startY, StudentWorld* stdwld)
: Harmer(startX, startY, stdwld, IID_WATER_POOL, "water pool")
{}

WaterPool::~WaterPool()
{}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// POISON IMPLEMENTATION /////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Poison::Poison(int startX, int startY, StudentWorld* stdwld)
: Harmer(startX, startY, stdwld, IID_POISON, "poison")
{}

Poison::~Poison()
{}


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
