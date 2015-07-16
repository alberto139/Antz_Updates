/* Antz
 * Antz Framework
 *  Neighbor
 *
 *
*/
#inlcude Guider

using namespace Antz;

class Neighbor{
 uint32_t MyNumber = 0;
 uint8_t id = 0;
 uint8_t curFood = 0xFF;
 uint8_t curNest = 0xFF;
 bool foodFound = false;
 uint8_t role = 0; // 0 means Worker
 
} 

Neighbor::Neighbor(number){
 id = (number >> 16) & 0xFF;
 curFood = (number >> 8) & 0xFF;
 curNest = number & 0xFF;
 foodFound = (number >> 24) & 1;
 role = (number >> 25) & 1; 
}


