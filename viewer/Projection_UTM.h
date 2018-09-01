// Projection_UTM.h: interface for the CProjection_UTM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTION_UTM_H__FB7283CC_DF19_46EF_898F_D6C661EE5B94__INCLUDED_)
#define AFX_PROJECTION_UTM_H__FB7283CC_DF19_46EF_898F_D6C661EE5B94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"

class CProjection_UTM : public CProjection  
{
public:
	CProjection_UTM();
	virtual ~CProjection_UTM();

	long For(double lon, double lat, double* x, double* y);
	long Inv(double x, double y, double* lon, double* lat);

	// non virtual functions
	long Init(double r_maj,	double r_min, double scale_fact, long zone);
	long InitTM(double r_maj,				/* major axis			*/
					double r_min,			/* minor axis			*/
					double scale_fact,		/* scale factor			*/
					double center_lon,		/* center longitude		*/
					double center_lat,		/* center latitude		*/
					double false_east,		/* x offset in meters		*/
					double false_north,		/* y offset in meters		*/
					double angular_unit,
					double linear_unit);	
};

#endif // !defined(AFX_PROJECTION_UTM_H__FB7283CC_DF19_46EF_898F_D6C661EE5B94__INCLUDED_)
