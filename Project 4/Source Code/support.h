#ifndef support_h
#define support_h

#include "provided.h"
#include "MyMap.h"
#include <iostream>

typedef bool (*comp)(int,int);
bool compare(const GeoCoord &c, const GeoCoord &d);

bool operator<(const GeoCoord &a, const GeoCoord &b);
bool operator==(const GeoCoord &a, const GeoCoord &b);
bool operator>(const GeoCoord &a, const GeoCoord &b);

// NOTE: THIS IS NOT A MEMBER FUNCTION OF NAVIGATOR; TO MAKE IT EASIER TO CONSTRUCT THE ROUTE I IMPLEMENTED THIS FUNCTION IN SUPPORT.CPP:
NavResult constructRoute(const GeoCoord &start, const GeoCoord &cur, const GeoCoord &last, SegmentMapper* segments, MyMap<GeoCoord, GeoCoord> &coords, std::vector<NavSegment> &dir);
//std::string directionOfLine(const GeoSegment current, const GeoSegment previous);

#endif
