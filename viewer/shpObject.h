// shpObject.h: interface for the shpObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHPOBJECT_H__85DE2690_DA15_4711_9089_C632258E05DE__INCLUDED_)
#define AFX_SHPOBJECT_H__85DE2690_DA15_4711_9089_C632258E05DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "shpDBF.h"
#include "ProgressBar.h"

class shpObject  
{
public:
	virtual bool		Open(CString _filename);
	virtual	void		Draw(CDC* _pDC, double _ZoomFactor, GPoint _ptMapCenter, const CRect& _rect, double _lfRatio);
	virtual int			Track(CWnd* pWnd, GPoint point);

	// Coordinate conversion between Geo Coordinate & Screen Coordinate
	CRect			WorldToDevice(const MBR&	_mbr);
	MBR				DeviceToWorld(const CRect& _rect);

	CPoint			WorldToDevice(const GPoint& _GPoint);
	GPoint			DeviceToWorld(const CPoint& _point);

	void			SetColor(COLORREF _color){m_objColor = _color;}
	void			SetBrushColor(COLORREF _color){m_brushColor = _color;}
	void			SetWidth(int _nWidth){m_nWidth = _nWidth;}
	void			SetStyle(int _nStyle){m_nStyle = _nStyle;}
	void			SetSolid(bool _bSolid){m_bSolidPolygon = _bSolid;}
	void			SetLabel(int _nField){m_nLabelField = _nField; m_bLabel = true;}
	void			RemoveLabel(){m_bLabel = false;}
	
	// DB Related & and so on..
	MBR				GetMBR(){return m_MBR;}
	int				GetSHPType(){return m_nShapeType;}
	int				GetRecordsNumber(){return m_nRecords;}
	int				GetFieldNumber(){return m_nField;}
	const char*		ReadStringAttribute(int _id, int _field){return m_DBF.ReadStringAttribute(_id, _field);}
	DBFFieldType	GetFieldInfo(int iField, char * pszFieldName, int * pnWidth, int * pnDecimals );

	shpObject();
	virtual ~shpObject();

public:
	shpDBF			m_DBF;						// DBF file...

	// hit test할 때 결과
	enum			hit_status	{hitNone, hitPoint, hitCurve, hitPolygon};
	enum			object_status {stateNone, stateCreate, stateSelect, stateDragCurve, stateDraCGPoint};

	typedef vector<GPoint>	POINT_LIST;

protected:
	void			SwapWord(int length, void* wordP);
	void			ReMBR(vector<GPoint>& _vPoint);
	bool			PtInRect(const GPoint& _geoInputPoint,const GPoint& _geoRectCenter);
	bool			PtInPolygon(vector<GPoint>& _vPoint, GPoint _GPoint);
	bool			IsPointNearLine(const GPoint& _a, const GPoint& _b, const GPoint& _p, double _d);

protected:
	vector<int>		m_vSelectedID;				// selected ID

	CString			m_strSHPPath;				//Name of SHP file
	CString			m_strSHXPath;				//Name of SHX file
	CString			m_strDBFPath;				//Name fo DBF file

    int				m_nShapeType;				/* SHPT_* */
    int				m_nRecords;					// Number of Records
    int				m_nField;					// Number of Fields
	int				m_nLabelField;				// Label...

	// Appearance of point
	int				m_nWidth;		// width
	int				m_nStyle;		// solid pen, dotted pen and so on..
	COLORREF		m_objColor;		// Color
	COLORREF		m_brushColor;	// brush color
	bool			m_bSolidPolygon;

	MBR				m_MBR;
	GPoint			m_geoCenterPoint;	// geoCenter Point
	CPoint			m_scrCenterPoint;	// screen Center point
	double			m_lfRatio;			// ratio between screen & map
	double			m_lfZoomFactor;		// Screen zoom factor

	// selecting & Labeling..
	object_status	m_eStatus;
	bool			m_bLabel;

	// Tracking & Hitting test
	int				m_nDetectPixelSize;
};

#endif // !defined(AFX_SHPOBJECT_H__85DE2690_DA15_4711_9089_C632258E05DE__INCLUDED_)
