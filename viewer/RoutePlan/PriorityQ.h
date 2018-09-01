#define MAX_SIZE 500000
#include "RouteData.h"
class Heap
{
public :
  Heap() : _heap(NULL), _size(0), _pq_size(0) {}
  ~Heap() { destroy(); }
public :
  int  insert(ExpCost* anode);
  int  size() { return _size; }
  int  getmemsize() { return (_heap == NULL) ? 0 :_pq_size*sizeof(ExpCost); }
  int  init();
  void destroy();
  void release();
  ExpNode*       get_min_node();
  ExpCost*       pointer() { return _heap; }

private :
  ExpCost* _heap;
  int      _size;
  int      _pq_size;
};

inline int Heap::init() 
{
  if (_heap == NULL) {
    _heap = (ExpCost *)malloc(sizeof(ExpCost) * MAX_SIZE);
    memset((void *)_heap, 0, sizeof(ExpCost) * MAX_SIZE);
    if (_heap == NULL) return -1;
    _pq_size = MAX_SIZE;
  }
  _size = 0;
  return 0;
}

inline int Heap::insert(ExpCost* anode)
{
  if (_size == _pq_size - 1) {
    return -1;
  }

  ++_size;
  int i = _size;
  while (i != 1) {
    if (anode->_cost >= _heap[i / 2]._cost) break;
    _heap[i] = _heap[i/2];
    i /= 2;
  }
  _heap[i] = *anode;
  return 0;
}

inline ExpNode* Heap::get_min_node()
{
  ExpCost temp, retnode;

  if (_size == 0) {
    return NULL;
  }

  retnode = _heap[1];
  temp = _heap[_size];
  _heap[_size]._cost = 0;
  _heap[_size]._my_node = 0;
  --_size;

  int i = 1;
  int j = 2;
  for (; j <= _size;) {
    if (j < _size) 
      if (_heap[j]._cost > _heap[j + 1]._cost) 
        j++;

    if (temp._cost <= _heap[j]._cost) break; 

    _heap[i] = _heap[j];

    i = j;
    j *= 2;
  }
  _heap[i] = temp;

  return retnode._my_node;
}

inline void Heap::destroy()
{
  if (_heap != NULL) {
    free(_heap);
    _heap = NULL;
  }
}

inline void Heap::release()
{
  if (_heap != NULL) {
    memset(_heap, 0, sizeof(ExpCost) * _pq_size); 
  }
  _size = 0;
}

