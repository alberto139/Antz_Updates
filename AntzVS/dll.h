#ifndef DLL_H
#define DLL_H
#define NULL 0




class DllElem{
 public:
  DllElem(void *itmPtr);
  void *item;
  DllElem *next;
  DllElem *prev;
};

class Dll {
 public:
  Dll();
  ~Dll();
  void PushFront(void *itm);
  void PushBack(void *itm);
  void *PopFront();
  void *PopBack();
  bool IsEmpty();
  int GetSize();
  void Insert(int index, void *itm);
  void *Remove(int index);
 private:
  int size;
  DllElem *head;
  DllElem *tail;

};



#endif
