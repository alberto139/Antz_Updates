#include "dll.h"

DllElem::DllElem(void *itemPtr)
{
  item = itemPtr;
  prev = NULL;
  next = NULL;

}


Dll::Dll()
{
  head = tail = NULL;
  size = 0;

}

Dll::~Dll(){
  while(PopFront() !=NULL)
    ;

}

void Dll::PushFront(void *item){
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



void Dll::PushBack(void *item){
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

void *Dll::PopFront(){

  if(size == 0)
    return NULL;
  else if(size == 1){
    void *ret = head->item;
    delete head;
    head = tail = NULL;
    size--;
    return ret;

  }else{
    DllElem *temp = head;
    head = head->next;
    head->prev = NULL;
    size--;
    void *ret = temp->item;
    delete temp;
    return ret;
    
  }

}
void *Dll::PopBack(){

  if(size == 0)
    return NULL;
  else if(size == 1){
    void *ret = head->item;
    delete head;
    head = tail = NULL;
    size--;
    return ret;

  }else{
    DllElem *temp = tail;
    tail = tail->prev;
    tail->next = NULL;
    size--;
    void *ret = temp->item;
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


void Dll::Insert(int index, void *item){
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

void *Dll::Remove(int index){
  if(index >= size)
    return NULL;
  else if(index == 0){
    void *ret = head->item;
    head = head->next;
    size--;
    return ret;
  }
  else if(index == (size - 1)){
    void *ret = tail->item;
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
    void *ret = sentinal->item;
    delete sentinal;
    return ret;

  }
}
