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
    class WorkerRole : public Role
    {
    public:
        WorkerRole(SmartBot& _robot);
        ~WorkerRole() {}

        int getRoleId() { return ROLE_WORKER; };
        int makeStep();

    protected:

        bool receiveSignal(int& roleDecision);

        void makeMovement();
        void randomWalkGo();
        void randomWalkReset();


        // Maybe we should change the variable names below

        uint64_t randomMoveTimer;
        uint8_t target;
        uint32_t curNumber;
        uint64_t numberTimer;
        uint8_t minSignal;
        uint8_t signalIndex;
        uint32_t minNumber;

        uint8_t foodIndex;
        uint8_t nestIndex;
        uint16_t movePhase;
        uint8_t noMoveCnt;

        int randomCircleCnt;
    };
}

#endif /* defined(__Antz__WorkerRole__) */
