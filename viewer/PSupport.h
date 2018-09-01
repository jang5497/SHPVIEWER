// PSupport.h: interface for the CPSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSUPPORT_H__522643F5_710F_42BE_BE9D_8090B971E1BF__INCLUDED_)
#define AFX_PSUPPORT_H__522643F5_710F_42BE_BE9D_8090B971E1BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* Projection codes

   0 = Geographic
   1 = Universal Transverse Mercator (UTM)
   2 = State Plane Coordinates
   3 = Albers Conical Equal Area
   4 = Lambert Conformal Conic
   5 = Mercator
   6 = Polar Stereographic
   7 = Polyconic
   8 = Equidistant Conic
   9 = Transverse Mercator
  10 = Stereographic
  11 = Lambert Azimuthal Equal Area
  12 = Azimuthal Equidistant
  13 = Gnomonic
  14 = Orthographic
  15 = General Vertical Near-Side Perspective
  16 = Sinusiodal
  17 = Equirectangular
  18 = Miller Cylindrical
  19 = Van der Grinten
  20 = (Hotine) Oblique Mercator 
  21 = Robinson
  22 = Space Oblique Mercator (SOM)
  23 = Alaska Conformal
  24 = Interrupted Goode Homolosine 
  25 = Mollweide
  26 = Interrupted Mollweide
  27 = Hammer
  28 = Wagner IV
  29 = Wagner VII
  30 = Oblated Equal Area
  99 = User defined
*/

/* Define projection codes */
#define GEO 0
#define UTM 1
#define SPCS 2
#define ALBERS 3
#define LAMCC 4
#define MERCAT 5
#define PS 6
#define POLYC 7
#define EQUIDC 8
#define TM 9
#define STEREO 10
#define LAMAZ 11
#define AZMEQD 12
#define GNOMON 13
#define ORTHO 14
#define GVNSP 15
#define SNSOID 16
#define EQRECT 17
#define MILLER 18
#define VGRINT 19
#define HOM 20
#define ROBIN 21
#define SOM 22
#define ALASKA 23
#define GOOD 24
#define MOLL 25
#define IMOLL 26
#define HAMMER 27
#define WAGIV 28
#define WAGVII 29
#define OBEQA 30
#define USDEF 99 

/* Define unit code numbers to their names */
#define RADIAN 0		/* Radians */
#define FEET 1			/* Feed */
#define METER 2			/* Meters */
#define SECOND 3		/* Seconds */
#define DEGREE 4		/* Decimal degrees */
#define INT_FEET 5		/* International Feet */

/* The STPLN_TABLE unit value is specifically used for State Plane -- if units
   equals STPLN_TABLE and Datum is NAD83--actual units are retrieved from
   a table according to the zone.  If Datum is NAD27--actual units will be feet.
   An error will occur with this unit if the projection is not State Plane.  */

#define STPLN_TABLE 6

/* General code numbers */

#define IN_BREAK -2		/*  Return status if the interupted projection
				    point lies in the break area */
#define COEFCT 15		/*  projection coefficient count */
#define PROJCT 30		/*  projection count */
#define SPHDCT 31		/*  spheroid count */

#define MAXPROJ 31		/*  Maximum projection number */
#define MAXUNIT 5		/*  Maximum unit code number */
#define GEO_TERM 0		/*  Array index for print-to-term flag */
#define GEO_FILE 1		/*  Array index for print-to-file flag */
#define GEO_TRUE 1		/*  True value for geometric true/false flags */
#define GEO_FALSE -1		/*  False val for geometric true/false flags */

///////////////////////////////////////////////////////////////////////////////////

#define PI 	3.141592653589793238
#define HALF_PI (PI*0.5)
#define TWO_PI 	(PI*2.0)
#define EPSLN	1.0e-10
#define R2D     57.2957795131
/*
#define D2R     0.0174532925199
*/
#define D2R   1.745329251994328e-2
#define S2R		4.848136811095359e-6

#define OK	0
#define SERROR  -1
#define IN_BREAK -2

/* Misc macros
  -----------*/
#define SQUARE(x)       x * x   /* x**2 */
#define CUBE(x)     x * x * x   /* x**3 */
#define QUAD(x) x * x * x * x   /* x**4 */

#define GMAX(A, B)      ((A) > (B) ? (A) : (B)) /* assign maximum of a and b */
#define GMIN(A, B)      ((A) < (B) ? (A) : (B)) /* assign minimum of a and b */

#define IMOD(A, B)      (A) - (((A) / (B)) * (B)) /* Integer mod function */

class CPSupport  
{
public:
	CPSupport();
	virtual ~CPSupport();

	static void sincos(double val, double *sin_val, double *cos_val);
	static double asinz(double con);
	static double msfnz(double eccent, double sinphi, double cosphi);
	static double qsfnz(double eccent, double sinphi, double cosphi);
	static double phi1z(double eccent, double qs, long  *flag);
	static double phi2z(double eccent, double ts, long *flag);
	static double phi3z(double ml,	double e0, double e1,	double e2, double e3,	long *flag);
	static double phi4z (double eccent, double e0, double e1, double e2, double e3, double a, double b, double* c, double* phi); 
	static double pakcz(double pak); /*Angle in alternate packed DMS format	*/
	static double pakr2dm(double pak); /* Angle in radians			*/
	static double tsfnz(double eccent, double phi, double sinphi);
	static long sign(double x);
	static double adjust_lon(double x); /* Angle in radians			*/
	static double e0fn(double x);
	static double e1fn(double x);
	static double e2fn(double x);
	static double e3fn(double x); 
	static double e4fn(double x);
	static double mlfn(double e0, double e1, double e2, double e3, double phi);
	static long calc_utm_zone(double lon);
	static void _datum_trans(double irmaj, double irmin, double ormaj, double ormin, 
														double ilon, double ilat, double ih, double *olon, double *olat, double *oh,
														double dx, double dy, double dz);

	static BOOL find_match_pos(std::string& text, const char* tokens, int pos, int* spos, int* epos);
	static BOOL find_single_match_pos(std::string& text, const char token, int pos, int* spos, int* epos);
	static void get_string_of_double(std::string& text, double value);

};

#endif // !defined(AFX_PSUPPORT_H__522643F5_710F_42BE_BE9D_8090B971E1BF__INCLUDED_)
