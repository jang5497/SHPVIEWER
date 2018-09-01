// Projection_UTM.cpp: implementation of the CProjection_UTM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "shpviewer.h"
#include "Projection_UTM.h"
#include "PSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CProjection_UTM::CProjection_UTM()
{

}

CProjection_UTM::~CProjection_UTM()
{

}

long CProjection_UTM::Init(double r_maj, double r_min, double scale_fact, long zone)
{

	double temp;			/* temprorary variables			*/

	if ((abs(zone) < 1) || (abs(zone) > 60))
	{
  		AfxMessageBox("Illegal zone number");
		return 0;
	}

	_r_major = r_maj;
	_r_minor = r_min;
	_scale_factor = scale_fact;
	_lat_origin = 0.0;
	_lon_center = ((6 * abs(zone)) - 183) * D2R;
	_false_easting = 500000.0;
	_false_northing = (zone < 0) ? 10000000.0 : 0.0;

	temp = _r_minor / _r_major;
	_es = 1.0 - SQUARE(temp);
	_e = sqrt(_es);
	_e0 = CPSupport::e0fn(_es);
	_e1 = CPSupport::e1fn(_es);
	_e2 = CPSupport::e2fn(_es);
	_e3 = CPSupport::e3fn(_es);
	_ml0 = _r_major * CPSupport::mlfn(_e0, _e1, _e2, _e3, _lat_origin);
	_esp = _es / (1.0 - _es);

	if (_es < .00001) _ind = 1;
	else _ind = 0;

	return 0;
}

/* Transverse Mercator forward equations--mapping lat,long to x,y
   Note:  The algorithm for TM is exactly the same as UTM and therefore
	  if a change is implemented, also make the change to UTMFOR.c
  --------------------------------------------------------------*/
long CProjection_UTM::For(double lon,	double lat,	double *x,	double *y)
{
	double delta_lon;	/* Delta longitude (Given longitude - center 	*/
	double sin_phi, cos_phi;/* sin and cos value				*/
	double al, als;		/* temporary values				*/
	double b;		/* temporary values				*/
	double c, t, tq;	/* temporary values				*/
	double con, n, ml;	/* cone constant, small m			*/

	/* Forward equations
		-----------------*/
	delta_lon = CPSupport::adjust_lon(lon - _lon_center);
	CPSupport::sincos(lat, &sin_phi, &cos_phi);

	/* This part was in the fortran code and is for the spherical form 
	----------------------------------------------------------------*/
	if (_ind != 0)
	{
		b = cos_phi * sin(delta_lon);
		if ((fabs(fabs(b) - 1.0)) < .0000000001)
		{
			//p_error("Point projects into infinity","tm-for");
			return(93);
		}
		else
		{
			*x = .5 * _r_major * _scale_factor * log((1.0 + b)/(1.0 - b));
			con = acos(cos_phi * cos(delta_lon)/sqrt(1.0 - b*b));
			if (lat < 0) con = - con;
			*y = _r_major * _scale_factor * (con - _lat_origin); 
			return(OK);
		}
	}

	al  = cos_phi * delta_lon;
	als = SQUARE(al);
	c   = _esp * SQUARE(cos_phi);
	tq  = tan(lat);
	t   = SQUARE(tq);
	con = 1.0 - _es * SQUARE(sin_phi);
	n   = _r_major / sqrt(con);
	ml  = _r_major * CPSupport::mlfn(_e0, _e1, _e2, _e3, lat);

	*x  = _scale_factor * n * al * (1.0 + als / 6.0 * (1.0 - t + c + als / 20.0 *
				(5.0 - 18.0 * t + SQUARE(t) + 72.0 * c - 58.0 * _esp))) + _false_easting;
	*y  = _scale_factor * (ml - _ml0 + n * tq * (als * (0.5 + als / 24.0 *
				(5.0 - t + 9.0 * c + 4.0 * SQUARE(c) + als / 30.0 * (61.0 - 58.0 * t
				+ SQUARE(t) + 600.0 * c - 330.0 * _esp))))) + _false_northing;

	return 0;
}

/* Transverse Mercator inverse equations--mapping x,y to lat,long 
   Note:  The algorithm for UTM is exactly the same as TM and therefore
          if a change is implemented, also make the change to UTMINV.c
  --------------------------------------------------------------*/
long CProjection_UTM::Inv(double x,	 double y,	double *lon,  double *lat)
{
	double con,phi;		/* temporary angles				*/
	double delta_phi;	/* difference between longitudes		*/
	long i;			/* counter variable				*/
	double sin_phi, cos_phi, tan_phi;	/* sin cos and tangent values	*/
	double c, cs, t, ts, n, r, d, ds;	/* temporary variables		*/
	double f, h, g, temp;			/* temporary variables		*/
	long max_iter = 6;			/* maximun number of iterations	*/

	/* fortran code for spherical form 
	--------------------------------*/
	if (_ind != 0)
	{
		f = exp(x/(_r_major * _scale_factor));
		g = .5 * (f - 1/f);
		temp = _lat_origin + y/(_r_major * _scale_factor);
		h = cos(temp);
		con = sqrt((1.0 - h * h)/(1.0 + g * g));
		*lat = CPSupport::asinz(con);
		if (temp < 0)
			*lat = -*lat;
		if ((g == 0) && (h == 0))
		{
			*lon = _lon_center;
			return(OK);
		}
		else
		{
			*lon = CPSupport::adjust_lon(atan2(g,h) + _lon_center);
			return 0;
		}
	}

	/* Inverse equations
		-----------------*/
	x = x - _false_easting;
	y = y - _false_northing;

	con = (_ml0 + y / _scale_factor) / _r_major;
	phi = con;
	for (i=0;;i++)
	{
		delta_phi = ((con + _e1 * sin(2.0*phi) - _e2 * sin(4.0*phi) + _e3 * sin(6.0*phi)) / _e0) - phi;
	/*
		 delta_phi = ((con + e1 * sin(2.0*phi) - e2 * sin(4.0*phi)) / e0) - phi;
	*/
		phi += delta_phi;
		if (fabs(delta_phi) <= EPSLN) break;
		if (i >= max_iter) 
		{ 
			//p_error("Latitude failed to converge","TM-INVERSE"); 
			return(95);
		}
	}

	if (fabs(phi) < HALF_PI)
	{
		CPSupport::sincos(phi, &sin_phi, &cos_phi);
		tan_phi = tan(phi);
		c    = _esp * SQUARE(cos_phi);
		cs   = SQUARE(c);
		t    = SQUARE(tan_phi);
		ts   = SQUARE(t);
		con  = 1.0 - _es * SQUARE(sin_phi); 
		n    = _r_major / sqrt(con);
		r    = n * (1.0 - _es) / con;
		d    = x / (n * _scale_factor);
		ds   = SQUARE(d);
		*lat = phi - (n * tan_phi * ds / r) * (0.5 - ds / 24.0 * (5.0 + 3.0 * t + 
						10.0 * c - 4.0 * cs - 9.0 * _esp - ds / 30.0 * (61.0 + 90.0 * t +
						298.0 * c + 45.0 * ts - 252.0 * _esp - 3.0 * cs)));
		*lon = CPSupport::adjust_lon(_lon_center + (d * (1.0 - ds / 6.0 * (1.0 + 2.0 * t +
						c - ds / 20.0 * (5.0 - 2.0 * c + 28.0 * t - 3.0 * cs + 8.0 * _esp +
						24.0 * ts))) / cos_phi));
	}
	else
	{
		*lat = HALF_PI * CPSupport::sign(y);
		*lon = _lon_center;
	}
	return 0;
}

long CProjection_UTM::InitTM(double r_maj,	double r_min,	double scale_fact,	double center_lon,	double center_lat,	double false_east,	double false_north,	double angular_unit, double linear_unit)
{
	double temp;			/* temporary variable		*/

	_angular_unit = angular_unit;
	_linear_unit = linear_unit;

	/* Place parameters in static storage for common use
  -------------------------------------------------*/
	_r_major = r_maj * _linear_unit;
	_r_minor = r_min * _linear_unit;
	_scale_factor = scale_fact;
	_lon_center = center_lon * _angular_unit;
	_lat_origin = center_lat * _angular_unit;
	_false_easting = false_east * _linear_unit;
	_false_northing = false_north * _linear_unit;

	temp = _r_minor / _r_major;
	_es = 1.0 - SQUARE(temp);
	_e = sqrt(_es);
	_e0 = CPSupport::e0fn(_es);
	_e1 = CPSupport::e1fn(_es);
	_e2 = CPSupport::e2fn(_es);
	_e3 = CPSupport::e3fn(_es);
	_ml0 = _r_major * CPSupport::mlfn(_e0, _e1, _e2, _e3, _lat_origin);
	_esp = _es / (1.0 - _es);

	if (_es < .00001) _ind = 1;
	else _ind = 0;

	return 0;
}