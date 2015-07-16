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
Neighbor::Neighbor(uint32_t number){
  id = (number >> 16) & 0xFF;
  curFood = (number >> 8) & 0xFF;
  curNest = number & 0xFF;
  foodFound = (number >> 24) & 1;
  role = (number >> 25) & 1; 
}





