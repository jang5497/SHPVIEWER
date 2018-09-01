// Projection.cpp: implementation of the CProjection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "shpviewer.h"
#include "Projection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjection::CProjection()
{
}

CProjection::~CProjection()
{
}

// Molodensky Datum Transformation function for general datum transformation.
void CProjection::DatumTrans(double irmaj,			// input major
														 double irmin,			// input major
														 double ilon,				// input longitude
														 double ilat,				// input latitude
														 double ih,					// z value(only different ellipsoid)
														 double ormaj, 
														 double ormin, 
														 double *olon,
														 double *olat,
														 double *oh, 
														 int dx, 
														 int dy,
														 int dz)
{
	double dmaj, df, dlat, dlon, dh;
	// differeces between input ellipsoid and output ellipsoid like major axis and flattening. As is ouput - input
	double rm, rn; //'radius of curvature in the meridian and radius of curvature in the prime vertical
	double temp, es_temp, f;

	temp = irmin / irmaj;
	f = 1 - temp;								// flattening
	es_temp = 1 - temp * temp;	// e2

	dmaj = ormaj - irmaj;								// output major axis - input major axis
	df = irmin / irmaj - ormin / ormaj;	// output Flattening - input flattening

	rm = irmaj * (1 - es_temp) / pow(1 - es_temp * sin(ilat) * sin(ilat), 1.5);
	rn = irmaj / pow(1 - es_temp * sin(ilat) * sin(ilat), .5);

	dlat = ((((-dx * sin(ilat) * cos(ilon) - dy * sin(ilat) * cos(ilon)) + dz * cos(ilat)) + dmaj * rn * es_temp * sin(ilat) * cos(ilat) / irmaj) + df * (rm / temp + rn * temp) * sin(ilat) * cos(ilat)) / (rm + ih);
	dlon = (-dx * sin(ilon) + dy * cos(ilon)) / ((rn + ih) * cos(ilat));
	dh = dx * cos(ilat) * cos(ilon) + dy * cos(ilat) * sin(ilon) + dz * sin(ilat) - dmaj * irmaj / rn + df * temp * rn * sin(ilat) * sin(ilat);

	*olat = ilat + dlat;
	*olon = ilon + dlon;
	*oh = ih + dh;
}

void CProjection::D2DMS(double _lon, double _lat, int& _lonD, int& _lonM, double& _lonS, int& _latD, int& _latM, double& _latS)
{
	_lonD = (int)_lon;
	_lonM = (int)((_lon - _lonD) * 60);
	_lonS = ((_lon - _lonD) * 60 - _lonM) * 60;

	_latD = (int)_lat;
	_latM = (int)((_lat - _latD) * 60);
	_latS = ((_lat - _latD) * 60 - _latM) * 60;
}