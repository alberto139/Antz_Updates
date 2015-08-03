/*
 * Antz
 * Antz Framework
 * Guider.h
 *		Guider is a type of robot which acts as beacon in the process of foraging
 *
 * Copyright: 2015 Ac Zhi Xing. All rights reserved.
 * License: <#license#>
 */

#ifndef __Antz__Guider__
#define __Antz__Guider__

#include "AntzRobot.h"

#define DEFAULT_PRIORITY 50

namespace Antz {
    class Guider: public AntzRobot {
    public:
        Guider(uint32_t robotId);
        virtual ~Guider() {}
        virtual void setup();
        virtual void loop();
        int Wcount; //counter for wiping Neighborhood
        //int neighborCount;
    protected:
        virtual bool receiveSignal();
        virtual void sendSignal();
        virtual void transition();
        bool isNeighborValid(Neighbor& neighbor);
        
        bool recalculation = false;
        
        
        uint16_t minFood;
        uint16_t minNest;
        int16_t curFood;
        int16_t curNest;
        uint64_t foodTimer;
        uint64_t nestTimer;
        uint8_t priority;
    };
}

#endif /* defined(__Antz__Guider__) */
