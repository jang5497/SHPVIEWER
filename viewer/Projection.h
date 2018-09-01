// Projection.h: interface for the CProjection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTION_H__4323ED57_87CF_4BC8_80A1_EE4C5CEC2B9F__INCLUDED_)
#define AFX_PROJECTION_H__4323ED57_87CF_4BC8_80A1_EE4C5CEC2B9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define UDEF	0
#define FOR		1
#define INV		2

class CProjection  
{
protected:
	double _r_major;				/* major axis 				*/
	double _r_minor;				/* minor axis 				*/
	double _scale_factor;		/* scale factor				*/
	double _lon_center;			/* Center longitude (projection center) */
	double _lat_origin;			/* center latitude			*/
	double _e0,_e1,_e2,_e3;	/* eccentricity constants		*/
	double _e,_es,_esp;			/* eccentricity constants		*/
	double _ml0;						/* small value m			*/
	double _false_northing;	/* y offset in meters			*/
	double _false_easting;	/* x offset in meters			*/
	double _ind;						/* spherical flag			*/
	double _angular_unit;
	double _linear_unit;

public:
	CProjection();
	virtual ~CProjection();

private:
	int _mode;

public:
	virtual long For(double lon, double lat, double* x, double* y) = 0;
	virtual long Inv(double x, double y, double* lon, double* lat) = 0;

	void DatumTrans(double irmaj,								// input major
									double irmin,				// input major
									double ilon,				// input longitude
									double ilat,				// input latitude
									double ih,
									double ormaj, 
									double ormin, 
									double* olon,
									double* olat,
									double* oh, 
									int dx, 
									int dy,
									int dz);

	void	D2DMS(double _lon, double _lat, int& _lonD, int& _lonM, double& _lonS, int& _latD, int& _latM, double& _latS);
};


#endif // !defined(AFX_PROJECTION_H__4323ED57_87CF_4BC8_80A1_EE4C5CEC2B9F__INCLUDED_)
