#ifndef _ROUTEDATA_
#define _ROUTEDATA_
#include <vector>
#include <map>
#include <math.h>
class Point
{
public :
  Point() : _x(0), _y(0) {}
  ~Point() {};
  Point(double x, double y) {
    _x = x;
    _y = y;
  }
  Point& operator=(const Point& lhs) {
	  _x = lhs._x;
	  _y = lhs._y;
	  return *this;
  }

  bool operator==(const Point& lhs) {
	  if (abs(_x - lhs._x) < 2 && abs(_y - lhs._y) < 2) {
		return true;
	  } else return false;
  }

public :
  double dist2Point(const Point& pt) {
    return sqrt(pow(_x - pt._x, 2) + pow(_y - pt._y, 2));
  }
  static double dist2Point(const Point& spt, const Point& ept) {
    return sqrt(pow(spt._x - ept._x, 2) + pow(spt._y - ept._y, 2));
  }
  double _x;
  double _y;
};

class nodeRecord 
{
public :
  nodeRecord() 
  {
    _f_pt._x = 0;
    _f_pt._y = 0;
    _t_pt._x = 0;
    _t_pt._y = 0;
    _connect_link.clear();
  };
  ~nodeRecord() 
  {
    if (_connect_link.size() > 0) {
      _connect_link.clear();
      std::vector<unsigned long>().swap(_connect_link);
    }
  };
  void push_link(unsigned long link) 
  {
    if (_connect_link.empty()) {
      _connect_link.push_back(link);
    } else {
      for (int i = 0; i < (int)_connect_link.size(); ++i) 
      {
        if (link == _connect_link[i]) break;
        if (i == _connect_link.size() -1) 
        {
          _connect_link.push_back(link);
        }
      }
    }
  }
public : 
  Point _f_pt, _t_pt;
  std::vector<unsigned long> _connect_link;
};

class linkRecord
{
public :
  linkRecord() : _f_node(0), _t_node(0), _length(0), _shppoly_idx(0) {};
  ~linkRecord() {
	  destroy();
  }
public:
	void calculate_length() 
	{
		if (_length == 0) 
		{
			for (int i = 0; i < (int)_point_vec.size()-1; ++i) 
			{
				_length += _point_vec[i].dist2Point(_point_vec[i+1]);
			}
		}
	}
	void destroy() {
		_point_vec.clear();
		std::vector<Point>().swap(_point_vec);
	}
	linkRecord& operator=(const linkRecord& lhs) {
		if (_point_vec.size() > 0) {
			destroy();
		}
		_f_node = lhs._f_node;
		_t_node = lhs._t_node;
		_length = lhs._length;
		_shppoly_idx = lhs._shppoly_idx;
		_point_vec.resize(lhs._point_vec.size());
		memcpy((char *)&_point_vec[0], (char *)&lhs._point_vec[0], sizeof(Point) * lhs._point_vec.size());
		return *this;
	}
public :
  unsigned long          _f_node;
  unsigned long          _t_node;
  double                 _length;
  int                    _shppoly_idx;
  std::vector<Point>     _point_vec;
};

class FlagPoint
{
public : 
  FlagPoint() : _linkid(0), _index(0) {};
public :
  unsigned long    _linkid;
  int              _index;
  Point           _pt;
};

class ExpNode {
public :
  ExpNode(): _nodeid(0), _linkid(0), _expand_distance(0), _dir(0), _parent(NULL) {}
public :
  unsigned long _nodeid;
  unsigned long _linkid;
  int           _dir;
  int           _expand_distance;
  ExpNode*      _parent;
};

class ExpCost {
public :
  ExpCost() : _my_node(NULL), _cost(0) {}
public :
  ExpNode* _my_node;
  int      _cost;
};

class CostTableData {
public :
  CostTableData() : _index(0), _cost(0) {}
  int _index;
  int _cost;
};


typedef std::map<unsigned long, nodeRecord>      nodeRecord_map;
typedef std::map<unsigned long, linkRecord>      linkRecord_map;
typedef std::pair<unsigned long, unsigned long > CosTableKey; // linkid, nodeid
typedef std::map<CosTableKey, CostTableData>              CostTable;
typedef std::map<CosTableKey, CostTableData>::iterator    CostTbliter;

#endif