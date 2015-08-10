/* Antz
 * Antz Framework
 *  Role.cpp
 *
*/

#include "Role.h"

using namespace Antz;

/* sendSignal -- Send signals from all the senders */
void Role::sendSignal()
{
    /* 
    *  message can be visualized as follows: ( - => 1 bit )
    *		| ------ | - | - | -------- | -------- | -------- |
    *		 NOT USED  1b        8b         8b         8b
    *		          role    identifier  curFood    curNest
    */
    uint32_t signalData = 0;
    uint32_t temp = getRoleId();
    signalData |= (temp << 25);
    signalData |= (robot.identifier << 16);
    signalData |= (robot.curFood << 8);
    signalData |= robot.curNest;
    robot.sender.send(signalData, 500);
    
}
