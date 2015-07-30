#ifndef DLL_H
#define DLL_H
#define NULL 0

#include "Neighbor.h"


namespace Antz
{
    class Dll;
    class DllIter;

    class DllElem
    {
        friend class Dll;
        friend class DllIter;
    private:
        DllElem(Neighbor& itmPtr);
        Neighbor* item;
        DllElem *next;
        DllElem *prev;
        
    };

    class DllIter
    {
        friend class Dll;
    public:
        Neighbor* getNext();
        bool hasNext();
    private:
        DllIter(Dll& _list);
        DllElem* getNextElem();

        Dll* list;
        DllElem* pointer;
    };

    class Dll
    {
        friend class DllIter;
    public:
        Dll();
        ~Dll();
        void PushFront(Neighbor& itm);
        void PushBack(Neighbor& itm);
        Neighbor* PopFront();
        Neighbor* PopBack();
        bool IsEmpty();
        int GetSize();
        void Insert(int index, Neighbor& itm);
        Neighbor* Remove(int index);
        bool remove(Neighbor& neighbor);
        DllIter* createIterator();
    private:
        int size;
        DllElem *head;
        DllElem *tail;

    };

}

#endif
