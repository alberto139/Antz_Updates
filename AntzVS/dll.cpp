#include "dll.h"

using namespace Antz;

DllElem::DllElem(Neighbor& itemPtr)
{
  item = &itemPtr;
  prev = NULL;
  next = NULL;
}

DllIter::DllIter(Dll& _list)
{
    list = &_list;
    pointer = list->head;
}

Neighbor* DllIter::getNext()
{
    DllElem* next = getNextElem();
    return next == NULL ? NULL : next->item;
}

bool DllIter::hasNext()
{
    return pointer != NULL;
}

DllElem* DllIter::getNextElem()
{
    if (!hasNext())
        return NULL;
    DllElem* toReturn = pointer;
    pointer = pointer->next;
    return toReturn;
}

Dll::Dll()
{
    head = tail =  NULL;
    size = 0;
}

Dll::~Dll()
{
    Neighbor* deleted;
    while ((deleted = popFront()) != NULL)
        delete deleted;
}

DllIter* Dll::createIterator()
{
    return new DllIter(*this);
}

void Dll::pushFront(Neighbor& neighbor)
{
    DllElem* newElem = new DllElem(neighbor);
    if (isEmpty())
    {
        head = newElem;
        tail = newElem;
    }
    else
    {
        newElem->next = head;
        head->prev = newElem;
        head = newElem;
    }
    size++;
}

void Dll::pushBack(Neighbor& neighbor)
{
    DllElem* newElem = new DllElem(neighbor);
    if (isEmpty())
    {
        head = newElem;
        tail = newElem;
    }
    else
    {
        newElem->prev = tail;
        tail->next = newElem;
        tail = newElem;
    }
    size++;
}

Neighbor* Dll::popFront()
{
    if (size == 0)
        return NULL;
    Neighbor* toReturn;
    if (size == 1)
    {
        toReturn = head->item;
        delete head;
        head = tail = NULL;
    }
    else
    {
        toReturn = head->item;
        head = head->next;
        delete head->prev;
        head->prev = NULL;
    }
    size--;
    return toReturn;

}

Neighbor* Dll::popBack()
{
    if (size == 0)
        return NULL;
    if (size == 1)
        return popFront();
    else
    {
        Neighbor* toReturn = tail->item;
        tail = tail->prev;
        delete tail->next;
        tail->next = NULL;
        size--;
        return toReturn;
    }
}

bool Dll::isEmpty()
{
    return size == 0;
}

int Dll::getSize()
{
    return size;
}

bool Dll::remove(DllElem* elem)
{
    if (elem == NULL)
        return false;

    if (elem->prev == NULL)
        head = elem->next;
    else
        elem->prev->next = elem->next;

    if (elem->next == NULL)
        tail = elem->prev;
    else
        elem->next->prev = elem->prev;
    size--;
    delete elem;
    return true;
}

bool Dll::remove(Neighbor& neighbor)
{
    DllIter* iter = createIterator();
    DllElem* toRemove = NULL;
    while (iter->hasNext() && toRemove == NULL)
    {
        DllElem* current = iter->getNextElem();
        if (current->item == &neighbor)
            toRemove = current;
    }
    delete iter;
    return remove(toRemove);
}

