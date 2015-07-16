/*
 * Antz
 * Antz Framework
 * Neighboor.h
 *		Structure for the Neighbor 
 * 
 */

#ifndef __Antz__Neighbor__
#define __Antz__Neighbor__

//#include "AntzRobot.h"
#include <stdint.h>

namespace Antz {
   class Neighbor {
     public:
     uint32_t MyNumber;
     int id;
     uint8_t curFood;
     uint8_t curNest;
     bool foodFound;
     uint8_t role; // 0 means Worker
     
     Neighbor();
     Neighbor(uint32_t neighbor);
   };
}
   
#endif
     
     
