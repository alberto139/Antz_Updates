/*
* Antz
* Antz Framework
* WorkerRole.h
*
* Copyright (c) 2015 All rights reserved.
* License: <#license#>
*/


#ifndef __Antz__LineRole__
#define __Antz__LineRole__


#include "AntzRobot.h"
#include "Role.h"

#define TARGET_NEST 0
#define TARGET_FOOD 1

namespace Antz
{
    class LineRole : public Role
    {
    public:
        LineRole(SmartBot& _robot);
        ~LineRole() {}

        int getRoleId() { return ROLE_WORKER; };
        int makeStep();

    protected:

        bool receiveSignal(int& roleDecision);

        void makeMovement();
        void randomWalkGo();
        void randomWalkReset();
        void forwardStep();


        // Maybe we should change the variable names below

        uint64_t randomMoveTimer;
        uint8_t target;
        uint32_t curNumber;
        uint64_t numberTimer;
        uint8_t maxSignal;
        uint8_t signalIndex;
        uint32_t maxNumber;

        uint8_t foodIndex;
        uint8_t nestIndex;
        uint16_t movePhase;
        uint8_t noMoveCnt;

        int randomCircleCnt;

        int predecessorId = -1;
        int lastSeenId = -1;
    };
}

#endif /* defined(__Antz__WorkerRole__) */
