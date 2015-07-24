/*
 * Antz
 * Antz Framework
 * WorkerRole.h
 *
 * Copyright (c) 2015 All rights reserved.
 * License: <#license#>
 */


#ifndef __Antz__GuiderRole__
#define __Antz__GuiderRole__


#include "AntzRobot.h"
#include "Role.h"

namespace Antz 
{
    class GuiderRole: public Role
    {
      public:
        GuiderRole(uint32_t robotId);
        virtual ~GuiderRole() {}
        virtual void setup();
        virtual void loop();
        int Wcount; //counter for wiping Neighborhood

        protected:
        //virtual bool receiveSignal(); // Defined in Role.h

       
        virtual void sendSignal();
        Sender sender;

         // Maybe we should change the variable names below
        uint16_t minFood;
        uint16_t minNest;
        uint16_t curFood;
        uint16_t curNest;
        uint64_t foodTimer;
        uint64_t nestTimer;
        uint8_t priority;
        virtual void makeMovement();
        virtual void randomWalkGo();
        virtual void randomWalkReset();
    
    };
}

#endif /* defined(__Antz__GuiderRole__) */
