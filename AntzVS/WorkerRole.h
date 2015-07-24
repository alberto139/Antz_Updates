/*
 * Antz
 * Antz Framework
 * WorkerRole.h
 *
 * Copyright (c) 2015 All rights reserved.
 * License: <#license#>
 */


#ifndef __Antz__WorkerRole__
#define __Antz__WorkerRole__


#include "AntzRobot.h"
#include "Role.h"

namespace Antz 
{
    class WorkerRole: public Role
    {
      public:
        WorkerRole(uint32_t robotId);
        virtual ~WorkerRole() {}
        virtual void setup();
        virtual void loop();

        protected:
        //virtual bool receiveSignal(); // Defined in Role.h
        virtual void makeMovement();
        virtual void randomWalkGo();
        virtual void randomWalkReset();


        // Maybe we should change the variable names below
        
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

#endif /* defined(__Antz__WorkerRole__) */
