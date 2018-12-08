#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{}

StudentWorld::~StudentWorld()
{}

int StudentWorld::init()
{
    
    Compiler *compilerForEntrant0, *compilerForEntrant1, *compilerForEntrant2, *compilerForEntrant3;
    
    std::vector<std::string> fileNames = GameWorld::getFilenamesOfAntPrograms();
    std::string error;
    
    bool ah0 = false;
    bool ah1 = false;
    bool ah2 = false;
    bool ah3 = false;
     
    switch(fileNames.size())
    {
        case 4:
        {
            compilerForEntrant3 = new Compiler;
            if ( ! compilerForEntrant3->compile(fileNames[3], error) )
            {
                setError(fileNames[3] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            m_compilers.push_back(compilerForEntrant3);
            ants.push_back(0);
            tickofant.push_back(0);
            ah3 = true;
        }
        case 3:
        {
            compilerForEntrant2 = new Compiler;
            if ( ! compilerForEntrant2->compile(fileNames[2], error) )
            {
                setError(fileNames[2] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            m_compilers.push_back(compilerForEntrant2);
            ants.push_back(0);
            tickofant.push_back(0);
            ah2 = true;
        }
        case 2:
        {
            compilerForEntrant1 = new Compiler;
            if ( ! compilerForEntrant1->compile(fileNames[1], error) )
            {
                setError(fileNames[1] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            m_compilers.push_back(compilerForEntrant1);
            ants.push_back(0);
            tickofant.push_back(0);
            ah1 = true;
        }
        case 1:
        {
            compilerForEntrant0 = new Compiler;
            if ( ! compilerForEntrant0->compile(fileNames[0], error) )
            {
                setError(fileNames[0] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            m_compilers.push_back(compilerForEntrant0);
            ants.push_back(0);
            tickofant.push_back(0);
            ah0 = true;
        }
        default:
            break;
    }
    
    for(int x = 0; x < 64; x++)
    {
        for(int y = 0; y < 64; y++)
        {
            if(!loadField(x, y, ah0, ah1, ah2, ah3))
                return GWSTATUS_LEVEL_ERROR;
        }
    }
    m_ticks = 0;
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    updateTickCount();
    resetMove();
    
    for(int x = 0; x < 64; x++)
    {
        for(int y = 0; y < 64; y++)
        {
            list<Actor*>::iterator actr = m_actors[x][y].begin();
                while(actr != m_actors[x][y].end())
                {
                    if(!(*actr)->movedThisTurn())
                    {
                        int oldX = (*actr)->getX(), oldY = (*actr)->getY();
                        (*actr)->doSomething();
                        if((*actr)->getX() != oldX || (*actr)->getY() != oldY)
                            {
                            updatePosition(*actr, oldX, oldY);
                            }
                    }
                    actr++;
                }
        }
    }
    
    for(int x = 0; x < 64; x++)
        for(int y = 0; y < 64; y++)
            removeTheDead(x, y);
    
    if(getCurrentTicks() == 2000)
    {
        if(winningAntNum() != -1)
        {
            setWinner(m_compilers[winningAntNum()]->getColonyName());
            return GWSTATUS_PLAYER_WON;
        }
        return GWSTATUS_NO_WINNER;
    }
    
    updateDisplayText();
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::updateTickCount()
{
    m_ticks++;
}

int StudentWorld::getCurrentTicks()
{
    return m_ticks;
}

void StudentWorld::updatePosition(Actor* actor, int oldx, int oldy)
{
    list<Actor*>::iterator actr = m_actors[oldx][oldy].begin();
    while(actr != m_actors[oldx][oldy].end())
    {
        if((*actr) == actor)
        {
            m_actors[actor->getX()][actor->getY()].push_back(actor);
            m_actors[oldx][oldy].erase(actr);
            break;
        }
        actr++;
    }
}

void StudentWorld::resetMove()
{
    for(int x = 0; x < 64; x++)
    {
        for(int y = 0; y < 64; y++)
        {
            list<Actor*>::iterator actr = m_actors[x][y].begin();
            
            while(actr != m_actors[x][y].end())
            {
                (*actr)->setNewMove();
                (*actr)->setBit(false);
                actr++;
            }
        }
    }
}

bool StudentWorld::canMove(int x, int y)
{
    if(x < 0 || x > 63 || y < 0 || y > 63)
        return false;
    list<Actor*>::iterator actr = m_actors[x][y].begin();
    while(actr != m_actors[x][y].end())
    {
        if((*actr)->blocksInsect())
            return false;
        actr++;
    }
    return true;
}

void StudentWorld::updateDisplayText()
{
    int ticks = getCurrentTicks();
    int antsAnt0, antsAnt1, antsAnt2, antsAnt3; int winningAntNumber;
    antsAnt0 = ants[0];
    antsAnt1 = ants[1];
    antsAnt2 = ants[2];
    antsAnt3 = ants[3];
    winningAntNumber = winningAntNum();
    string s = formatDisplayText(ticks, antsAnt0, antsAnt1, antsAnt2, antsAnt3, winningAntNumber);
    setGameStatText(s);
    return;
}

std::string StudentWorld::formatDisplayText(int ticks, int ants0, int ants1, int ants2, int ants3, int winningAntNum)
{
    string s = "Ticks: ";
    if(2000 - ticks < 10)
        s+= "   ";
    else if(2000 - ticks < 100)
        s+= "  ";
    else if(2000 - ticks < 1000)
        s+= " ";
    
    s += to_string(2000 - ticks);
    s += " - ";
    
    for(int i = 0; i < ants.size(); i++)
    {
        if(i != 0)
            s += "  ";
        s += m_compilers[i]->getColonyName();
        if(i == winningAntNum)
            s += "*";
        s += ": ";
        if(ants[i] < 10)
            s += "0";
        s += to_string(ants[i]);
        s += " ants";
    }
    
    return s;
}


bool StudentWorld::loadField(int x, int y, bool ah0, bool ah1, bool ah2, bool ah3)
{
    Field f;
    std::string fieldFile = getFieldFilename();
    if (f.loadField(fieldFile) != Field::LoadResult::load_success)
        return false;
    
    Field::FieldItem item = f.getContentsOf(x,y); // note it’s x,y and not y,x!!!
    
    if (item == Field::FieldItem::rock)
    {
        Actor *p = new Pebble(x, y, this);
        m_actors[x][y].push_back(p);
    }
    else if (item == Field::FieldItem::anthill0 && ah0)
    {
        Actor *ah0 = new AntHill(x, y, this, "anthill 0", m_compilers[0]);
        m_actors[x][y].push_back(ah0);
    }
    else if (item == Field::FieldItem::anthill1 && ah1)
    {
        Actor *ah1 = new AntHill(x, y, this, "anthill 1", m_compilers[1]);
        m_actors[x][y].push_back(ah1);
    }
    else if (item == Field::FieldItem::anthill2 && ah2)
    {
        Actor *ah2 = new AntHill(x, y, this, "anthill 2", m_compilers[2]);
        m_actors[x][y].push_back(ah2);
    }
    else if (item == Field::FieldItem::anthill3 && ah3)
    {
        Actor *ah3 = new AntHill(x, y, this, "anthill 3", m_compilers[3]);
        m_actors[x][y].push_back(ah3);
    }
    else if (item == Field::FieldItem::grasshopper)
    {
        Actor *bs = new BabyGrasshopper(x, y, this);
        m_actors[x][y].push_back(bs);
    }
    else if (item == Field::FieldItem::food)
    {
        Actor *f = new Food(x,y, this, 6000);
        m_actors[x][y].push_back(f);
    }
    else if (item == Field::FieldItem::water)
    {
        Actor *wp = new WaterPool(x, y, this);
        m_actors[x][y].push_back(wp);
    }
    else if (item == Field::FieldItem::poison)
    {
        Actor *psn = new Poison(x,y, this);
        m_actors[x][y].push_back(psn);
    }
    return true;
}

void StudentWorld::decomposeInsect(int x, int y, int foodUnits)
{
    Actor* f = new Food(x, y, this, foodUnits);
    findActor(x, y).push_back(f);
}

std::list<Actor*> StudentWorld::findActor(int x, int y)
{
    return m_actors[x][y];
}

void StudentWorld::removeTheDead(int x, int y)
{
    list<Actor*>::iterator actr = m_actors[x][y].begin();
    while(actr != m_actors[x][y].end())
    {
        if((*actr)->isDead())
        {
            list<Actor*>::iterator temp = actr;
            actr = m_actors[x][y].erase(temp);
            delete (*temp);
        }
        else
            actr++;
    }
    return;
}

void StudentWorld::setAnts(int numberAnts, int colonyID)
{
    if(colonyID < 0 || colonyID > 3)
        return;
    else
        ants[colonyID] = numberAnts;
}

void StudentWorld::setTickAnt(int tick, int colonyID)
{
    if(colonyID < 0 || colonyID > 3)
        return;
    else
        tickofant[colonyID] = tick;
}

int StudentWorld::winningAntNum()                                                           //////////////////
{
    int winningNumber = 0;
    
    for(int i = 1; i < 4; i++)
    {
        if(ants[i] > ants[i - 1])
            winningNumber = i;
        else if(ants[i] == ants[winningNumber])
        {
            if(tickofant[i] < tickofant[winningNumber])
                winningNumber = i;
        }
    }
    if(winningNumber == 0 && tickofant[1] == tickofant[0])
        return -1;
    return winningNumber;
}

int StudentWorld::insectEat(int x, int y, int eatThisMuch)
 {
     std::list<Actor*>::iterator actr = m_actors[x][y].begin();
     while(actr != m_actors[x][y].end())
     {
        if((*actr)->typeOfActor() == "food" && !(*actr)->isDead())
        {
            return((*actr)->setHitPts(-eatThisMuch));
        }
        actr++;
     }
     return 0;
 }

bool StudentWorld::antCheckPheromone(int x, int y, std::string type)
{
    std::list<Actor*>::iterator actr = m_actors[x][y].begin();
    while(actr != m_actors[x][y].end())
    {
        if(type == "ant 0")
            if((*actr)->typeOfActor() == "pheromone 0")
                return true;
        if(type == "ant 1")
            if((*actr)->typeOfActor() == "pheromone 1")
                return true;
        if(type == "ant 2")
            if((*actr)->typeOfActor() == "pheromone 2")
                return true;
        if(type == "ant 3")
            if((*actr)->typeOfActor() == "pheromone 3")
                return true;
        actr++;
    }
    return false;
}

bool StudentWorld::antCheckInsect(int x, int y, std::string type)
{
    std::list<Actor*>::iterator actr = m_actors[x][y].begin();
    while(actr != m_actors[x][y].end())
    {
        if((*actr)->typeOfActor() == "grasshopper" || (*actr)->typeOfActor() == "baby grasshopper")
            return true;
        if(type == "ant 0")
            if((*actr)->typeOfActor() == "ant 1" || (*actr)->typeOfActor() == "ant 2" || (*actr)->typeOfActor() == "ant 3")
                return true;
        if(type == "ant 1")
            if((*actr)->typeOfActor() == "ant 0" || (*actr)->typeOfActor() == "ant 2" || (*actr)->typeOfActor() == "ant 3")
                return true;
        if(type == "ant 2")
            if((*actr)->typeOfActor() == "ant 0" || (*actr)->typeOfActor() == "ant 1" || (*actr)->typeOfActor() == "ant 3")
                return true;
        if(type == "ant 3")
            if((*actr)->typeOfActor() == "ant 0" || (*actr)->typeOfActor() == "ant 1" || (*actr)->typeOfActor() == "ant 2")
                return true;
        actr++;
    }
    return false;
}

bool StudentWorld::insectCheckObject(int x, int y, std::string type)
{
    std::list<Actor*>::iterator actr = m_actors[x][y].begin();
    while(actr != m_actors[x][y].end())
    {
        if((*actr)->typeOfActor() == type)
        {
            return true;
        }
        actr++;
    }
    return false;
}

void StudentWorld::newFood(int x, int y, int amt)
{
    std::list<Actor*>::iterator actr = m_actors[x][y].begin();
    while(actr != m_actors[x][y].end())
    {
        if((*actr)->typeOfActor() == "food")
        {
            (*actr)->setHitPts(amt);
            return;
        }
        actr++;
    }
    
    Actor* f = new Food(x, y, this, amt);
    m_actors[x][y].push_back(f);
}

void StudentWorld::newPheromone(int x, int y, std::string image)
{
    std::list<Actor*>::iterator actr = m_actors[x][y].begin();
    while(actr != m_actors[x][y].end())
    {
        if((*actr)->typeOfActor() == image)
        {
            if((*actr)->getEnergyPoints() >= 512)
                (*actr)->setHitPts(768 - (*actr)->getEnergyPoints());
            else
                (*actr)->setHitPts(256);
            return;
        }
        actr++;
    }
    
    int ID = -1;
    
    if(image == "pheromone 0")
        ID = IID_PHEROMONE_TYPE0;
    if(image == "pheromone 1")
        ID = IID_PHEROMONE_TYPE1;
    if(image == "pheromone 2")
        ID = IID_PHEROMONE_TYPE2;
    if(image == "pheromone 3")
        ID = IID_PHEROMONE_TYPE3;
    
    Actor* ph = new Pheromone(x, y, this, ID, image);
    m_actors[x][y].push_back(ph);
}

void StudentWorld::newAnt(int x, int y, Actor* a)
{
    m_actors[x][y].push_back(a);
    a->setMoved();
}

void StudentWorld::newGrasshopper(int x, int y)
 {
    Actor *g = new Grasshopper(x, y, this, IID_ADULT_GRASSHOPPER, "grasshopper");
    m_actors[x][y].push_back(g);
    g->setMoved();
 }

 bool StudentWorld::antgrasshopperbite(Actor* itself, int x, int y)
 {
    bool hasBitten = false;
    std::vector<Actor*> enemies;
    std::list<Actor*>::iterator actr = m_actors[x][y].begin();
    while(actr != m_actors[x][y].end())
    {
        if((*actr) != itself)
        {
            if((*actr)->typeOfActor() == "baby grasshopper" || (*actr)->typeOfActor() == "ant 0" || (*actr)->typeOfActor() == "ant 1" || (*actr)->typeOfActor() == "ant 2"|| (*actr)->typeOfActor() == "ant 3" || (*actr)->typeOfActor() == "grasshopper")
            {
                enemies.push_back((*actr));
                hasBitten = true;
            }
        }
        actr++;
    }
     if(hasBitten)
     {
         Actor* bitten = enemies[randInt(0, enemies.size() - 1)];
         bitten->setBit(true);
         if(itself->typeOfActor() == "grasshopper")
             bitten->setHitPts(-50);
         else
             bitten->setHitPts(-15);
         if(bitten->typeOfActor() == "grasshopper" && ! bitten->isDead())
             bitten->retaliate();
     }
     return hasBitten;
 }

void StudentWorld::cleanUp()
{
    for(int x = 0; x < 64; x++)
        for(int y = 0; y < 64; y++)
        {
            if(! m_actors[x][y].empty())
            {
                list<Actor*>::iterator actr = m_actors[x][y].begin();
                while(actr != m_actors[x][y].end())
                {
                    list<Actor*>::iterator temp = actr;
                    if((*temp) != nullptr)
                        delete (*temp);
                    actr++;
                }
            }
        }
    for(int i = 0; i < m_compilers.size(); i++)
        delete m_compilers[i];
}
