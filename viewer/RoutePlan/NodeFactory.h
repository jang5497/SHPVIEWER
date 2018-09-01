#ifndef  _NODEFACTORY_
#define  _NODEFACTORY_
#define MAX_FACTORY 3000000
#include "RouteData.h"
class NodeFactory
{
public :
  NodeFactory() : _node_storage(NULL), _size(0), _pos(-1) {}
  ~NodeFactory() { destroy(); }
  int init () {
    _node_storage = (ExpNode *)malloc(sizeof(ExpNode ) * MAX_FACTORY);
    memset((void *)_node_storage, 0, sizeof(ExpNode ) * MAX_FACTORY);
    if (_node_storage == NULL) return -1;
    else return 0;
  }
  void destroy() {
    if (_node_storage != NULL) {
      free(_node_storage);
      _node_storage = NULL;
      _size = 0;
    }
  }
  int size() { return _size; }

  ExpNode* get_node(int &index) {
    if (_node_storage != NULL) {
      index = ++_pos;
      if (_pos > MAX_FACTORY) {
        return NULL;
      } else {
        return &_node_storage[_pos];
      }
    } else return NULL;
  }

private :
  ExpNode* _node_storage;
  int       _size;
  int       _pos;
};
#endif