/* Antz
 * Antz Framework
 *  Neighbor.cpp
 *
 *
*/
#include "Neighbor.h"

using namespace Antz;

/* 
 * Neighbor -- constructor 
 *	Initialize all
 */
Neighbor::Neighbor(uint32_t signal)
{
    orgSignal = signal;
    id = (signal >> 16) & 0xFF;
    curFood = (signal >> 8) & 0xFF;
    curNest = signal & 0xFF;
    foodFound = (signal >> 24) & 1;
    role = (signal >> 25) & 1;
    for (int i =0;i<6;i++){
      recievedFrom[i] = 0;
    }
}





