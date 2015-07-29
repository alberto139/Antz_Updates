#include "dll.h"

using namespace Antz;

DllElem::DllElem(Neighbor& itemPtr)
{
  item = &itemPtr;
  prev = NULL;
  next = NULL;
  

}


Dll::Dll()
{
  head = tail = iter =  NULL;
  size = 0;
  

}

Dll::~Dll(){
  while(PopFront() !=NULL)
    ;

}

void Dll::intiIter(){
 iter = head;
  
}

Neighbor* Dll::getNext(){
 Neighbor* neighbor = iter->item;
 iter = iter->next;
 return neighbor;
  
}

void Dll::PushFront(Neighbor& item){
  if(IsEmpty())
    {
      DllElem *temp = new DllElem(item);
      head = temp;
      tail = temp;
      size++;

    }else{
    DllElem *temp = new DllElem(item);
    temp->next = head;
    head->prev = temp;
    head = temp;
    size++;
  }

}



void Dll::PushBack(Neighbor& item){
  if(IsEmpty())
    {
      DllElem *temp = new DllElem(item);
      head = temp;
      tail = temp;
      size++;

    }else{
    DllElem *temp = new DllElem(item);
    temp->prev = head;
    tail->next = temp;
    tail = temp;
    size++;
  }

}

Neighbor* Dll::PopFront(){

  if(size == 0)
    return NULL;
  else if(size == 1){
    Neighbor* ret = head->item;
    delete head;
    head = tail = NULL;
    size--;
    return ret;

  }else{
    DllElem *temp = head;
    head = head->next;
    head->prev = NULL;
    size--;
    Neighbor* ret = temp->item;
    delete temp;
    return ret;
    
  }

}
Neighbor* Dll::PopBack(){

  if(size == 0)
    return NULL;
  else if(size == 1){
    Neighbor* ret = head->item;
    delete head;
    head = tail = NULL;
    size--;
    return ret;

  }else{
    DllElem *temp = tail;
    tail = tail->prev;
    tail->next = NULL;
    size--;
    Neighbor* ret = temp->item;
    delete temp;
    return ret;

  }

}

bool Dll::IsEmpty(){
  return (size == 0);
}


int Dll::GetSize(){
  return size;
}


void Dll::Insert(int index, Neighbor& item){
  if(index+1 > size)
    ;
  else{
    DllElem *temp = new DllElem(item);
    DllElem *sentinal = head;
    int i = 0;
    
    while(i< index){
      sentinal = sentinal->next;
      i++;
    }
    temp->prev = sentinal->prev;
    temp->next = sentinal;
    sentinal->prev->next = temp;
    sentinal->prev = temp;
    size++;
    delete temp;
    delete sentinal;
  }

}

Neighbor* Dll::Remove(int index){
  if(index >= size)
    return NULL;
  else if(index == 0){
    Neighbor* ret = head->item;
    head = head->next;
    size--;
    return ret;
  }
  else if(index == (size - 1)){
    Neighbor* ret = tail->item;
    tail = tail->prev;
    size--;
    return ret;

  }

  else{
    DllElem *sentinal = head;
    int i = 0;
    while(i<index){
      sentinal = sentinal->next;
      i++;
    }
    sentinal->prev->next = sentinal->next;
    sentinal->next->prev = sentinal->prev;
    size--;
    Neighbor* ret = sentinal->item;
    delete sentinal;
    return ret;

  }
}