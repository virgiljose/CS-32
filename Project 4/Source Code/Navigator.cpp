#include "provided.h"
#include "support.h"
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <iostream>
#include "MyMap.h"
using namespace std;

GeoCoord finalCoords;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    MapLoader* navLoad;
    SegmentMapper* m_segments;
    AttractionMapper* navAttraction;
};


NavigatorImpl::NavigatorImpl()
{
    navLoad = new MapLoader;
    m_segments = new SegmentMapper;
    navAttraction = new AttractionMapper;
}

NavigatorImpl::~NavigatorImpl()
{
    delete navLoad;
    delete m_segments;
    delete navAttraction;
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    if(navLoad->load(mapFile))
    {
        m_segments->init(*navLoad);
        navAttraction->init(*navLoad);
//        std::cerr << "there are " << navLoad->getNumSegments() << "segments" << std::endl;
        return true;
    }
    else
        return false;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    directions.clear();                                         // empty the direction vector if needed;
    
    // VARIABLES AND DATA STRUCTURES FOR NAV IMPLEMENTATION:
    
        // variables:
        GeoCoord startCoords;
        GeoCoord currentCoords;
        int howmanytimes = 0;                                   // debugging purposes; count how many times something is pushed into a vector
    
        // data structures used to help navigate
        vector<StreetSegment> StreetVector;                     // at each given GeoCoord, find the StreetSegments associated with it.
        vector<StreetSegment> finalStreets;                     // StreetSegments associated with the final GeoCoord
        vector<GeoCoord> GeoCoordVector;                        // for each StreetSegment associated with a given GeoCoord, store their StreetSegments' associated GeoCoords
        struct compare
        {
            bool operator() (GeoCoord const &c, GeoCoord const &d)
            {
                double distA = sqrt( pow(c.latitude - finalCoords.latitude, 2) + pow(c.longitude - finalCoords.longitude, 2));
                double distB = sqrt( pow(d.latitude - finalCoords.latitude, 2) + pow(d.longitude - finalCoords.longitude, 2));
            
                return (distA > distB);
            }
        };
        std::priority_queue<GeoCoord, vector<GeoCoord>, compare> GeoCoordQueue;     // queue the GeoCoords
        MyMap<GeoCoord, GeoCoord> navDirections;                                    // map each GeoCoord with the previous GeoCoord
        MyMap<GeoCoord, int> navVisited;                                            // map which GeoCoords we already visited
 
    // START OF NAV IMPLEMENTATION:
    if(!navAttraction->getGeoCoord(start, startCoords))     // get GeoCoords associated with source
    { return NAV_BAD_SOURCE; }
    if(!navAttraction->getGeoCoord(end, finalCoords))       // NOTE: finalCoords is declared globally so that the priority queue comparison operator can use it
    { return NAV_BAD_DESTINATION; }
    
    finalStreets = m_segments->getSegments(finalCoords);
    
    navDirections.associate(startCoords, startCoords);
    navVisited.associate(startCoords, 1);
    GeoCoordQueue.push(startCoords);

    while(!GeoCoordQueue.empty())
    {
        currentCoords = GeoCoordQueue.top();                       // dequeue the top GeoCoord
        GeoCoordQueue.pop();
 
        GeoCoordVector.clear();
        StreetVector = m_segments->getSegments(currentCoords);     // get street segments associated with that GeoCoord
        
        for(int i = 0; i < StreetVector.size(); i++)
        {
            for(int j = 0; j < finalStreets.size(); j++)           // check if any GeoCoords associated with any those street segments are associated with the final destination
            {
                if(StreetVector[i].segment.start.latitude == finalStreets[j].segment.start.latitude && StreetVector[i].segment.start.longitude == finalStreets[j].segment.start.longitude && StreetVector[i].segment.end.latitude == finalStreets[j].segment.end.latitude && StreetVector[i].segment.end.longitude == finalStreets[j].segment.end.longitude)
                    return constructRoute(startCoords, currentCoords, finalCoords, m_segments, navDirections, directions);
                if(StreetVector[i].segment.start.latitude == finalStreets[j].segment.end.latitude && StreetVector[i].segment.start.longitude == finalStreets[j].segment.end.longitude && StreetVector[i].segment.end.latitude == finalStreets[j].segment.start.latitude && StreetVector[i].segment.end.longitude == finalStreets[j].segment.start.longitude)
                    return constructRoute(startCoords, currentCoords, finalCoords, m_segments, navDirections, directions);
//                if(StreetVector[i].segment.end.latitude == finalStreets[j].segment.start.latitude && StreetVector[i].segment.end.longitude == finalStreets[j].segment.start.longitude)
//                    return constructRoute(startCoords, currentCoords, finalCoords, m_segments, navDirections, directions);
//                if(StreetVector[i].segment.end.latitude == finalStreets[j].segment.end.latitude && StreetVector[i].segment.end.longitude == finalStreets[j].segment.end.longitude)
//                    return constructRoute(startCoords, currentCoords, finalCoords, m_segments, navDirections, directions);
            }
            GeoCoordVector.push_back(StreetVector[i].segment.start);    // if not, push back those GeoCoords
            GeoCoordVector.push_back(StreetVector[i].segment.end);
        }
        
        for(int i = 0; i < GeoCoordVector.size(); i++)
        {
            if(navVisited.find(GeoCoordVector[i]) == NULL)
            {
                GeoCoordQueue.push(GeoCoordVector[i]);           // push GeoCoords into queue
                navDirections.associate(GeoCoordVector[i], currentCoords);
                ++howmanytimes;
                navVisited.associate(GeoCoordVector[i], 1);
            }
        }

    }
    
	return NAV_NO_ROUTE;
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
