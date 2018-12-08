#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

class AttractionMapperImpl
{
public:
    AttractionMapperImpl();
    ~AttractionMapperImpl();
    void init(const MapLoader& ml);
    bool getGeoCoord(string attraction, GeoCoord& gc) const;
    
private:
    MyMap<string, GeoCoord> m_attractionMap;    // this map holds strings cooresponding to the names of the attractions,
    int numAttr;
};                                              // NOT the Attraction class itself.

AttractionMapperImpl::AttractionMapperImpl()
{}

AttractionMapperImpl::~AttractionMapperImpl()
{}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    for(int i = 0; i < ml.getNumSegments(); i++)
    {
        StreetSegment streets;
        if(ml.getSegment(i, streets))
        {
            for(int j = 0; j < streets.attractions.size(); j++)
            {
                m_attractionMap.associate( streets.attractions[j].name, streets.attractions[j].geocoordinates );
                ++numAttr;
            }
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
//    std::transform(attraction.begin(), attraction.end(), attraction.begin(), ::tolower);
    if(m_attractionMap.find(attraction) != NULL)
    {
        gc = *m_attractionMap.find(attraction);
        return true;
    }
    else
        return false;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
