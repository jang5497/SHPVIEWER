#include "RoutePlan.h"

int RoutePlan::init_route(FlagPoint &start_point, FlagPoint &end_point) {
  if (_nodefatory.init() != 0) return INIT_FAIL;
  if (_priorityQ.init() != 0) return INIT_FAIL;
  _start_point = start_point;
  _end_point = end_point;
  _total_dist = Point::dist2Point(_start_point._pt, _end_point._pt);
  _reader = Reader::instance();
  _route_init = 1;
  return 0;
}
int RoutePlan::onelink_route() {

  linkRecord linkrec = _reader->_linkRecord_map.find(_end_point._linkid)->second;
  int min_dist = 9999999;
  int dist = 0;
  Point min_point;
  for (int i = 0; i < linkrec._point_vec.size(); ++i) {
    dist = Point::dist2Point(linkrec._point_vec[i], _start_point._pt);
    if (min_dist > dist) {
      min_dist = dist;
      min_point = linkrec._point_vec[i];
      _start_point._index = i;
    }
  }

  for (int i = 0; i < linkrec._point_vec.size(); ++i) {
    dist = Point::dist2Point(linkrec._point_vec[i], _end_point._pt);
    if (min_dist > dist) {
      min_dist = dist;
      min_point = linkrec._point_vec[i];
      _end_point._index = i;
    }
  }

  if (_start_point._index == _end_point._index) {
    return -1;
  }

  _result_vec.resize(1);
  int start_index = 0, end_index = 0;
  if (_start_point._index > _end_point._index) {
    start_index = _end_point._index;
    end_index   = _start_point._index;
  } else {
    start_index = _start_point._index;
    end_index   = _end_point._index;
  }
  for (int i = start_index; i <= end_index; ++i) {
    _result_vec[0]._point_vec.push_back(linkrec._point_vec[i]);
  }
  return 0;
}
void RoutePlan::start_expand() {
  ExpNode *expnode = NULL;
  ExpCost  expcost;
  linkRecord linkrec;
  nodeRecord noderec;
  int min_dist = 9999999;
  int dist = 0;
  Point min_point;
  //f_node setting;

  linkrec = _reader->_linkRecord_map.find(_start_point._linkid)->second;
  noderec = _reader->_nodeRecord_map.find(linkrec._t_node)->second;

  expnode = _nodefatory.get_node(_tbldata._index);
  expnode->_linkid = _start_point._linkid;
  expnode->_nodeid = linkrec._t_node;
  expnode->_dir = check_dir(expnode->_nodeid, linkrec);
  _visit_count++;
  if (expnode->_dir != DIRECTION::NO_DIR) {
    for (int i = 0; i < linkrec._point_vec.size(); ++i) {
      dist = Point::dist2Point(linkrec._point_vec[i], _start_point._pt);
      if (min_dist > dist) {
        min_dist = dist;
        min_point = linkrec._point_vec[i];
        _start_point._index = i;
      }
    }
    //_start_point._pt = min_point;

    expnode->_expand_distance = Point::dist2Point(_start_point._pt, noderec._t_pt);
    _tbldata._cost = expnode->_expand_distance;
    //테이블에 아무것도 없음 find x
    _costtable.insert(CostTable::value_type(CosTableKey(expnode->_linkid, expnode->_nodeid), _tbldata));
    expcost._my_node = expnode;
    expcost._cost = calculatecost(expnode);
    _priorityQ.insert(&expcost);
  }


  expnode = _nodefatory.get_node(_tbldata._index);
  expnode->_linkid = _start_point._linkid;
  expnode->_nodeid = linkrec._f_node;
  expnode->_dir = expnode->_dir = check_dir(expnode->_nodeid, linkrec);
  _visit_count++;
  if (expnode->_dir != DIRECTION::NO_DIR) {
    noderec = _reader->_nodeRecord_map.find(linkrec._f_node)->second;
    expnode->_expand_distance = Point::dist2Point(_start_point._pt, noderec._f_pt);
    _tbldata._cost = expnode->_expand_distance;

    _costtable.insert(CostTable::value_type(CosTableKey(expnode->_linkid, expnode->_nodeid), _tbldata));
    expcost._my_node = expnode;
    expcost._cost = calculatecost(expnode);
    _priorityQ.insert(&expcost);
  }

  linkrec = _reader->_linkRecord_map.find(_end_point._linkid)->second;
  min_dist = 9999999;
  dist = 0;
  for (int i = 0; i < linkrec._point_vec.size(); ++i) {
    dist = Point::dist2Point(linkrec._point_vec[i], _end_point._pt);
    if (min_dist > dist) {
      min_dist = dist;
      min_point = linkrec._point_vec[i];
      _end_point._index = i;
    }
  }
  //_end_point._pt = min_point;
}
int RoutePlan::check_dir(int nodeid, linkRecord linkrec) {
  nodeRecord noderec;
  int start_dist = 0, end_dist = 0;
  noderec = Reader::instance()->_nodeRecord_map.find(nodeid)->second;
  if (Point::dist2Point(noderec._f_pt, noderec._t_pt) > linkrec._length+100) {
    return DIRECTION::NO_DIR;
  }

  start_dist = Point::dist2Point(linkrec._point_vec[0], noderec._f_pt);
  end_dist = Point::dist2Point(linkrec._point_vec[linkrec._point_vec.size() -1], noderec._f_pt);
  if (start_dist > end_dist) {
    return DIRECTION::FOR_DIR;
  } else {
    return DIRECTION::BACK_DIR;
  }
}

int RoutePlan::check_distnace(ExpNode *expnode) {
  int distance = 0;
  Point prev_pt, next_pt;
  linkRecord prev_linkrec, next_linkrec;
  prev_linkrec = Reader::instance()->_linkRecord_map.find(expnode->_parent->_linkid)->second;
  if (expnode->_parent->_dir == DIRECTION::FOR_DIR) {
    prev_pt = prev_linkrec._point_vec[prev_linkrec._point_vec.size()-1];
  } else {
    prev_pt = prev_linkrec._point_vec[0];
  }
  next_linkrec = Reader::instance()->_linkRecord_map.find(expnode->_linkid)->second;
  if (expnode->_dir == DIRECTION::FOR_DIR) {
    next_pt = next_linkrec._point_vec[next_linkrec._point_vec.size()-1];
  } else {
    next_pt = next_linkrec._point_vec[0];
  }
  distance = Point::dist2Point(prev_pt, next_pt);
  return distance;
}

ExpNode* RoutePlan::made_nextnode(ExpNode *expnode, unsigned long linkid) {
  ExpNode* next_node = _nodefatory.get_node(_tbldata._index);
  next_node->_parent = expnode;
  next_node->_linkid = linkid;
  next_node->_expand_distance = expnode->_expand_distance;
  linkRecord_map::iterator iter;
  iter = _reader->_linkRecord_map.find(linkid);
  if (iter->second._f_node == expnode->_nodeid) {
    next_node->_nodeid = iter->second._t_node;
    next_node->_dir = check_dir(next_node->_nodeid, iter->second);
  } else {
    next_node->_nodeid = iter->second._f_node;
    next_node->_dir = check_dir(next_node->_nodeid, iter->second);
  }
  if (check_distnace(next_node) > (iter->second._length + 100)  || next_node->_dir == DIRECTION::NO_DIR) {
    return NULL;
  }

  next_node->_expand_distance += (int)iter->second._length;
  return next_node;
}

int RoutePlan::node_expand(ExpNode *expnode) {
  ExpNode* next_node = NULL;
  ExpCost expcost;
  nodeRecord_map::iterator iter;
  CostTbliter tbl_iter;
  int q_cost = 0;
  iter = _reader->_nodeRecord_map.find(expnode->_nodeid);
  for (int i = 0; i < iter->second._connect_link.size(); ++i) {
    _visit_count++;
    if (_end_route != NULL) {
      _wait_count++;
    }
    next_node = made_nextnode(expnode, iter->second._connect_link[i]);
    if (next_node == NULL) continue;
    q_cost = calculatecost(next_node);
    if (q_cost > _total_dist * 1.5 && _visit_count > 100000) continue;
    _tbldata._cost = next_node->_expand_distance;
    tbl_iter = _costtable.find(CosTableKey(next_node->_linkid, next_node->_nodeid));
    if (tbl_iter != _costtable.end()) {
      //데이터가 있다면
      int distance = tbl_iter->second._cost;
      if (tbl_iter->second._cost <= _tbldata._cost) continue;
      else {
        //데이터 갱신
        _costtable[CosTableKey(next_node->_linkid, next_node->_nodeid)] = _tbldata;
      }
    } else {
      _costtable.insert(CostTable::value_type(CosTableKey(next_node->_linkid, next_node->_nodeid), _tbldata));
    }
    expcost._cost = q_cost;
    expcost._my_node = next_node;
    _priorityQ.insert(&expcost);
  }
  return 0;
}

int  RoutePlan::calculatecost(ExpNode *expnode) 
{
  int cost = 0;
  int remain_dist = 0;

  int remain_dist_for = 0;
  int remain_dist_back = 0;

  nodeRecord_map::iterator node_iter;
  linkRecord_map::iterator link_iter;

  link_iter = _reader->_linkRecord_map.find(expnode->_linkid);
  int max_idx = link_iter->second._point_vec.size()-1;
  if (expnode->_dir == DIRECTION::FOR_DIR) {
    remain_dist = Point::dist2Point(link_iter->second._point_vec[max_idx], _end_point._pt);
  } else {
    remain_dist = Point::dist2Point(link_iter->second._point_vec[0], _end_point._pt);
  }
  cost = (expnode->_expand_distance) + remain_dist;
  return cost;
}

int  RoutePlan::Route() {
  if (_route_init == 0) return INIT_FAIL;
  if (_start_point._linkid == _end_point._linkid) {
    if (onelink_route() == 0) {
      _is_onelink = 1;
      return 0;
    } else return ONELINK_FAIL;
  }
  start_expand();
  ExpNode* expnode = NULL;
  int min_cost = 99999999;
  while ((expnode = _priorityQ.get_min_node()) != NULL) {
    if (expnode->_linkid == _end_point._linkid) {
      if (min_cost > expnode->_expand_distance) {
        min_cost = expnode->_expand_distance;
        _end_route = expnode;
      }
    }
    if (_wait_count > 3000) {
      break;
    }
    if (_priorityQ.size() == MAX_SIZE) return OVER_EXPAND;
    else if (_visit_count >= 2500000) return OVER_EXPAND;
    node_expand(expnode);
  }
  if (_end_route == NULL) return ROUTE_FAIL;
  //결과 벡터비교
  return 0;
}

void RoutePlan::make_result() {
  linkRecord linkrec;
  ExpNode *temp_node;
  int s_dir = 0, e_dir = 0;
  int end_idx = 0;
  if (_is_onelink == 0) {
    temp_node = _end_route;
    e_dir = temp_node->_dir;

    while (temp_node != NULL) {
      linkrec = Reader::instance()->_linkRecord_map.find(temp_node->_linkid)->second;
      _result_vec.push_back(linkrec);
      if (temp_node->_parent == NULL) s_dir = temp_node->_dir;
      temp_node = temp_node->_parent;
    }
    //경로를 뒤집는다
    for (int i = 0, j = _result_vec.size() - 1; i < j; ++i, --j) {
      linkrec = _result_vec[j];
      _result_vec[j] = _result_vec[i];
      _result_vec[i] = linkrec;
    }
    if (s_dir == DIRECTION::FOR_DIR) {
      _result_vec[0]._point_vec.erase(_result_vec[0]._point_vec.begin(), _result_vec[0]._point_vec.begin() + _start_point._index);
    } else {
      _result_vec[0]._point_vec.erase(_result_vec[0]._point_vec.begin() + _start_point._index +1, _result_vec[0]._point_vec.begin() + _result_vec[0]._point_vec.size());
    }

    end_idx = _result_vec.size() -1;

    if (e_dir == DIRECTION::FOR_DIR) {
      _result_vec[end_idx]._point_vec.erase(_result_vec[end_idx]._point_vec.begin() + _end_point._index +1, _result_vec[end_idx]._point_vec.begin() + _result_vec[end_idx]._point_vec.size());
    } else {
      _result_vec[end_idx]._point_vec.erase(_result_vec[end_idx]._point_vec.begin(), _result_vec[end_idx]._point_vec.begin() + _end_point._index);
    }
  }

  FILE *fp = fopen("./result.bin", "wb");
  if (fp != NULL) {
    for (int i = 0; i < _result_vec.size(); ++i) {
      int size = _result_vec[i]._point_vec.size();
      fwrite((char *)&size, sizeof(int), 1, fp);
      fwrite((char *)&_result_vec[i]._point_vec[0], sizeof(Point), _result_vec[i]._point_vec.size(), fp);
    }
    fclose(fp);
  }
}