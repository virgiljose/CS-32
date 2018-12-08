#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <vector>

using namespace std;

class SegmentMapperImpl
{
public:
    SegmentMapperImpl();
    ~SegmentMapperImpl();
    void init(const MapLoader& ml);
    vector<StreetSegment> getSegments(const GeoCoord& gc) const;
    
private:
    MyMap<GeoCoord, vector<StreetSegment> > m_segmentMap;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}


void SegmentMapperImpl::init(const MapLoader& ml)
{
     for(int i = 0; i < ml.getNumSegments(); i++)
     {
     
        StreetSegment streets;
        ml.getSegment(i, streets);
     
        vector<StreetSegment> theSegments;
        theSegments.push_back(streets);
        
         
        // Associate starting GeoCoord of the segment
        if(m_segmentMap.find(streets.segment.start) != NULL)
        {
            m_segmentMap.find(streets.segment.start)->push_back(streets);
        }
        else
        {
            m_segmentMap.associate(streets.segment.start, theSegments);
        }
     
        // Associate ending GeoCoord of the segment
        if(m_segmentMap.find(streets.segment.end) != NULL)
        {
            m_segmentMap.find(streets.segment.end)->push_back(streets);
        }
        else
        {
            m_segmentMap.associate(streets.segment.end, theSegments);
        }
        // Associate GeoCoords of any attractions associated with the segment
        for(int j = 0; j < streets.attractions.size(); j++)
        {
            if(m_segmentMap.find(streets.attractions[j].geocoordinates) != NULL)
            {
                m_segmentMap.find(streets.attractions[j].geocoordinates)->push_back(streets);
            }
            else
            {
                m_segmentMap.associate(streets.attractions[j].geocoordinates, theSegments);
            }
        }
     
     }
     
    return;
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    vector<StreetSegment> nullStreet;
    
     if(m_segmentMap.find(gc) != NULL)
     {
        return *m_segmentMap.find(gc);
     }
    else
        return nullStreet;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
