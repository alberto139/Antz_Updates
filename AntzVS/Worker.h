/*
 * Antz
 * Antz Framework
 * Worker.h
 *
 * Copyright (c) 2015 Zhi Xing. All rights reserved.
 * License: <#license#>
 */


#ifndef __Antz__Worker__
#define __Antz__Worker__

#include "AntzRobot.h"

namespace Antz {
    class Worker: public AntzRobot {
    public:
        Worker(uint32_t robotId);
        virtual ~Worker() {}
        virtual void setup();
        virtual void loop();
    protected:
        virtual bool receiveSignal();
        virtual void makeMovement();
        virtual void randomWalkGo();
        virtual void randomWalkReset();

        uint64_t randomMoveTimer;
        uint8_t target;
        uint32_t curNumber;
        uint64_t numberTimer;
        uint8_t minSignal;
        uint8_t signalIndex;
        uint32_t minNumber;
        uint8_t minFood;
        uint8_t minNest;
        uint8_t curFood;
        uint8_t curNest;
        uint8_t foodIndex;
        uint8_t nestIndex;
        uint16_t movePhase;
        uint8_t noMoveCnt;
    };
}

#endif /* defined(__Antz__Worker__) */
