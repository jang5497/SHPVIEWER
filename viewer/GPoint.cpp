// GPoint.cpp: implementation of the GPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "shpviewer.h"
#include "GPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GPoint::~GPoint()
{

}

GPoint& GPoint::operator=(const GPoint& _pt)
{	
	x = _pt.x;
	y = _pt.y;

	return *this;
}

GPoint& GPoint::operator=(const Point& _pt)
{	
	x = _pt._x;
	y = _pt._y;

	return *this;
}

GPoint& GPoint::operator-=(const GPoint& _pt)
{
	x = x - _pt.x;
	y = y - _pt.y;
	
	return *this;
}

GPoint& GPoint::operator+=(const GPoint& _pt)
{
	x = x + _pt.x;
	y = y + _pt.y;
	
	return *this;
}

bool GPoint::operator==(const GPoint& _pt)
{
	return ((x == _pt.x) && (y == _pt.y));
}

bool GPoint::operator!=(const GPoint& _pt)
{
	return !(*this == _pt);
}

double 	GPoint::dist2Point(const GPoint& _pt)	
{
	return sqrt(pow(x - _pt.x, 2) + pow(y - _pt.y, 2));
}
