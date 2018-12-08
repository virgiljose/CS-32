#include "provided.h"
#include "support.h"
#include <string>
#include <algorithm>
#include <fstream>     // used to read from files
#include <iostream>

using namespace std;

class MapLoaderImpl
{
public:
    MapLoaderImpl();
    ~MapLoaderImpl();
    bool load(string mapFile);
    size_t getNumSegments() const;
    bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<StreetSegment> m_streets;
    int m_numsegments;
};

MapLoaderImpl::MapLoaderImpl()
{
    m_numsegments = 0;
}

MapLoaderImpl::~MapLoaderImpl()
{
    // DO THIS (too lazy 2 implement rn)
}

bool MapLoaderImpl::load(string mapFile)
{
    ifstream file;
    file.open(mapFile, std::ifstream::in);
    
    if(!file.is_open())
    {
        cerr << (file.rdstate() & std::ifstream::failbit) << endl;
        return false;
    }
    
    while(!file.eof())
    {
        StreetSegment LA;
        
        // Get street name associated with StreetSegment Object
        std::string street;
        getline(file, street, '\n');
        if(street == "")
            break;
//        std::transform(street.begin(), street.end(), street.begin(), ::tolower);
        LA.streetName = street;
        
        // Get GeoCoords associated with StreetSegment Object
            std::string ilatTxt;
            getline(file, ilatTxt, ',');
//            cerr << ilatTxt << endl;
            LA.segment.start.latitudeText = ilatTxt;
            LA.segment.start.latitude = stod(ilatTxt);
        
            std::string ilonTxt;
            getline(file, ilonTxt, ' '); // because there's an initial space
            if(ilonTxt == "")
                getline(file, ilonTxt, ' ');
//            cerr << ilonTxt << endl;
            LA.segment.start.longitudeText = ilonTxt;
            LA.segment.start.longitude = stod(ilonTxt);
        
            std::string flatTxt;
            getline(file, flatTxt, ',');
//            cerr << flatTxt << endl;
            LA.segment.end.latitudeText = flatTxt;
            LA.segment.end.latitude = stod(flatTxt);
        
            std::string flonTxt;
            getline(file, flonTxt, '\n');
//            cerr << flonTxt << endl;
            LA.segment.end.longitudeText = flonTxt;
            LA.segment.end.longitude = stod(flonTxt);
        
        m_numsegments++;
        
        // If attractions exist, then also add them to StreetSegment object

        std::string numAttractions;
        getline(file, numAttractions, '\n');
        int numAtt = stoi(numAttractions);
        Attraction FogoDeCao;
        
        if(numAtt > 0)
        {
            for(int i = 1; i <= numAtt; i++)
            {
                
                // Get name of Attraction:
                std::string attrName;
                getline(file, attrName, '|');
//                std::transform(attrName.begin(), attrName.end(), attrName.begin(), ::tolower);
                FogoDeCao.name = attrName;
                
                // Get GeoCoords of Attraction:
                    std::string latTxt;
                    getline(file, latTxt, ',');
                    FogoDeCao.geocoordinates.latitudeText = latTxt;
//                    cerr << latTxt << endl;
                    FogoDeCao.geocoordinates.latitude = stod(latTxt);
                
                    std::string lonTxt;
//                    getline(file, lonTxt, ' '); stod skips whitespaces
                    getline(file, lonTxt, '\n');
                    FogoDeCao.geocoordinates.longitudeText = lonTxt;
//                    cerr << lonTxt << endl;
                    FogoDeCao.geocoordinates.longitude = stod(lonTxt);
                
                // Add Attraction to StreetSegment object's vector of Attractions:
                LA.attractions.push_back(FogoDeCao);
            }
        }
        m_streets.push_back(LA);
    }
	return true;
}


size_t MapLoaderImpl::getNumSegments() const
{
	return m_numsegments;
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if(segNum < getNumSegments())
    {
        seg = m_streets[segNum];
        return true;
    }
    else
        return false;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
