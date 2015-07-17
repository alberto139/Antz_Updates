/*
 * Antz
 * Antz Framework
 * Trnasiton.h
 *
 *
 * Copyright: 2015 Ac Alberto Rivera. All rights reserved.
 * License: <#license#>
 */

#ifndef ____Transition__
#define ____Transition__

//#include <stdio.h>

#include "AntzRobot.h"

#define DEFAULT_PRIORITY 50

namespace Antz {
    class Transition: public AntzRobot {
    public:
        Transition(uint32_t robotId);
        virtual ~Transition() {}
        virtual void setup();
        virtual void loop();
        int counter;            // Counter for wiping Neighborhood
        int neighborCount;
    protected:
        //virtual bool receiveSignal();
        //virtual void sendSignal();
        Sender sender;
        uint16_t minFood;
        uint16_t minNest;
        uint16_t curFood;
        uint16_t curNest;
        uint64_t foodTimer;
        uint64_t nestTimer;
        uint8_t priority;
    };
}

#endif /* defined(____Transition__) */
