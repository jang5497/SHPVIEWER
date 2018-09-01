#ifndef _READER_
#define _READER_
#include "RouteData.h"
class Reader
{
public :
  Reader() {
    _instance = NULL;
  };

  ~Reader() {
    destory();
  }

  static Reader* instance() {
    if (_instance == NULL) {
      _instance = new Reader;
      atexit(destroy_bridge);
      return _instance;
    }
    return _instance;
  }
  void destory() {
    if (!_nodeRecord_map.empty()){
      _nodeRecord_map.clear();
      nodeRecord_map().swap(_nodeRecord_map);
      _linkRecord_map.clear();
      linkRecord_map().swap(_linkRecord_map);
    }
  }

private : 
  static void destroy_bridge() {
    if (_instance != NULL) {
      Reader::instance()->destory();
      delete _instance;
      _instance = NULL;
    }
  }
private :
  static Reader* _instance;
public :
  nodeRecord_map _nodeRecord_map;
  linkRecord_map _linkRecord_map;
};
#endif