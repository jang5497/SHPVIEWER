#include "Reader.h"
#include "PriorityQ.h"
#include "NodeFactory.h"
#define ROUTE_SUCCESS 0x0
#define INIT_FAIL     0x1
#define OVER_EXPAND   0x2
#define ROUTE_FAIL    0x3
#define ONELINK_FAIL  0x4

class RoutePlan
{
public :
  enum DIRECTION { FOR_DIR, BACK_DIR, NO_DIR };
  RoutePlan() : _route_init(0), _visit_count(0), _wait_count(0), _total_dist(0), _is_onelink(0), _reader(NULL), _end_route(NULL) {}
  ~RoutePlan() {
    _result_vec.clear();
    std::vector<linkRecord>().swap(_result_vec);
  }
  int Route();
  int init_route(FlagPoint&, FlagPoint&);
  ExpNode* made_nextnode(ExpNode *, unsigned long linkid);
  ExpNode* get_start();
  int      check_distnace(ExpNode *);
  int node_expand(ExpNode *);
  int calculatecost(ExpNode *);
  int check_dir(int , linkRecord);
  void start_expand();
  int  onelink_route();
  void make_result();

private :
  Reader*       _reader;
  CostTable     _costtable;
  CostTableData _tbldata;
  Heap        _priorityQ;
  NodeFactory   _nodefatory;
  FlagPoint     _start_point, _end_point;
  ExpNode*      _end_route;
  std::vector<linkRecord> _result_vec; 
  int           _visit_count;
  int           _wait_count;
  int           _total_dist;
  int           _is_onelink;
  int           _route_init;
};
