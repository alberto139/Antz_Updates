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

namespace Antz
{
    class Neighbor
    {
    public:
        Neighbor() { id = -1; } // temporary
        Neighbor(uint32_t signal);

        int mostlySeenFrom();

        uint32_t orgSignal;
        int id;
        int curFood;
        int curNest;
        bool foodFound;
        uint8_t role;
        int receivedFrom[6] = { 0 };
   };
}
   
#endif
     
     
