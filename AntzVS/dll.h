#ifndef DLL_H
#define DLL_H
#define NULL 0

#include "Neighbor.h"


namespace Antz
{
    class DllElem{
    public:
        DllElem(Neighbor& itmPtr);
        Neighbor* item;
        DllElem *next;
        DllElem *prev;
    };

    class Dll {
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
    private:
        int size;
        DllElem *head;
        DllElem *tail;

    };

}

#endif
