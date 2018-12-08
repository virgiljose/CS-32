#include "support.h"

bool operator<(const GeoCoord &a, const GeoCoord &b)
{
    if( a.latitude < b.latitude)
        return true;
    else if (a.latitude == b.latitude && a.longitude < b.longitude)
        return true;
    else
        return false;
}

bool operator==(const GeoCoord &a, const GeoCoord &b)
{
    if( a.latitude == b.latitude && a.longitude == b.longitude)
        return true;
    else
        return false;
}

bool operator>(const GeoCoord &a, const GeoCoord &b)
{
    if( a.latitude > b.latitude)
        return true;
    else if (a.latitude == b.latitude && a.longitude > b.longitude)
        return true;
    else
        return false;
}

bool operator!=(const GeoCoord &a, const GeoCoord &b)
{
    if( a.latitude != b.latitude)
        return true;
    else if (a.latitude != b.latitude && a.longitude != b.longitude)
        return true;
    else
        return false;
}

NavResult constructRoute(const GeoCoord &start, const GeoCoord &cur, const GeoCoord &last, SegmentMapper* segments, MyMap<GeoCoord, GeoCoord> &coords, std::vector<NavSegment> &dir)
{
    std::vector<StreetSegment> tempcur;
    std::vector<StreetSegment> tempprev;
    
    std::vector<StreetSegment> constructSegments;
    
    std::vector<NavSegment> nav;
    
    coords.associate(last, cur);
    
    GeoCoord currr = last;
    GeoCoord prev;
    
//    std::cerr << "Final GeoCoord: " << last.latitude << "," << last.longitude << std::endl;
//    std::cerr << "Start GeoCoord: " << start.latitude << "," << start.longitude << std::endl;
    
    int k = 0;
    
    tempcur.clear();
    tempcur = segments->getSegments(last);
    constructSegments.push_back(tempcur[0]);
    
    for(;;)
    {
            prev = *coords.find(currr);
        
            tempcur.clear();
            tempcur = segments->getSegments(currr);
        
            tempprev.clear();
            tempprev = segments->getSegments(prev);
        
            std::cerr << "evaluating GeoCoords: " << currr.latitude << "," << currr.longitude << " and " << prev.latitude << "," << prev.longitude << std::endl;
            ++k;
        
            for(int i = 0; i < tempcur.size(); i++)
            {
                for(int j = 0; j < tempprev.size(); j++)
                {
                    if((tempcur[i].segment.start == tempprev[j].segment.start && tempcur[i].segment.end == tempprev[j].segment.end )||
                       (tempcur[i].segment.end == tempprev[j].segment.start && tempcur[i].segment.start == tempprev[j].segment.end))
                    {
                        std::cerr << "segment " << k << " pushed back" << std::endl;
                        constructSegments.push_back(tempcur[i]);
                    }
                }
            }
        if(prev == start)
            break;
        currr = *coords.find(currr);
    }

    for(int k = constructSegments.size() - 1; k > 0; k--)
    {
            std::string directionTravel;
            double angleTravel = angleOfLine(constructSegments[k].segment);
            if(angleTravel < 0)
                angleTravel += 360;
            angleTravel = angleTravel - 180;                                    // subtracted 180 because i'm putting in the segments backwards
            if(angleTravel < 0)
                angleTravel += 360;
            if(angleTravel >= 0 && angleTravel <= 22.5)
                directionTravel = "east";
            else if(angleTravel > 22.5 && angleTravel <= 67.5)
                directionTravel = "northeast";
            else if(angleTravel > 67.5 && angleTravel <= 112.5)
                directionTravel = "north";
            else if(angleTravel > 112.5 && angleTravel <= 157.5)
                directionTravel = "northwest";
            else if(angleTravel > 157.5 && angleTravel <= 202.5)
                directionTravel = "west";
            else if(angleTravel > 202.5 && angleTravel <= 247.5)
                directionTravel = "southwest";
            else if(angleTravel > 247.5 && angleTravel <= 292.5)
                directionTravel = "south";
            else if(angleTravel > 292.5 && angleTravel <= 337.5)
                directionTravel = "southeast";
            else if(angleTravel > 337.5 && angleTravel <= 360)
                directionTravel = "east";
            if(k == constructSegments.size() - 1)
                nav.push_back(NavSegment(directionTravel, constructSegments[k].streetName, distanceEarthMiles(start, constructSegments[k].segment.end), constructSegments[k].segment));
            else if(k == 1)
                nav.push_back(NavSegment(directionTravel, constructSegments[k].streetName, distanceEarthMiles(last, constructSegments[k].segment.end), constructSegments[k].segment));
            else
                nav.push_back(NavSegment(directionTravel, constructSegments[k].streetName, distanceEarthMiles(constructSegments[k].segment.start, constructSegments[k].segment.end), constructSegments[k].segment));
        if(constructSegments[k].streetName != constructSegments[k - 1].streetName)
        {
            std::string directionTravel;
            double angleTravel = angleBetween2Lines(constructSegments[k].segment, constructSegments[k - 1].segment) - 180;
            if(angleTravel >= 0 && angleTravel < 180)
                directionTravel = "left";
            else
                directionTravel = "right";
            nav.push_back(NavSegment(directionTravel, constructSegments[k].streetName));
            nav[nav.size() - 1].m_distance = 0;
        }
    }
    dir = nav;
    return NAV_SUCCESS;
}
/*
double directionOfLine(const GeoSegment current, const GeoSegment previous)
{
    std::string directionTravel;
    double angleTravel = angleOfLine(current);
    if(angleTravel < 0)
        angleTravel += 360;
    angleTravel = angleTravel - 180;                                    // subtracted 180 because i'm putting in the segments backwards
    if(angleTravel < 0)
        angleTravel += 360;
    if(angleTravel >= 0 && angleTravel <= 22.5)
        directionTravel = "east";
    else if(angleTravel > 22.5 && angleTravel <= 67.5)
        directionTravel = "northeast";
    else if(angleTravel > 67.5 && angleTravel <= 112.5)
        directionTravel = "north";
    else if(angleTravel > 112.5 && angleTravel <= 157.5)
        directionTravel = "northwest";
    else if(angleTravel > 157.5 && angleTravel <= 202.5)
        directionTravel = "west";
    else if(angleTravel > 202.5 && angleTravel <= 247.5)
        directionTravel = "southwest";
    else if(angleTravel > 247.5 && angleTravel <= 292.5)
        directionTravel = "south";
    else if(angleTravel > 292.5 && angleTravel <= 337.5)
        directionTravel = "southeast";
    else if(angleTravel > 337.5 && angleTravel <= 360)
        directionTravel = "east";

    return angleTravel;
}
*/
