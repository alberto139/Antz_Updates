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

        int getRoleId();
        int makeStep();

    protected:
        bool receiveSignal(int& roleDecision);

        void makeMovement();
        void forwardStep();

        uint8_t target;
        uint64_t numberTimer;
        uint8_t signalIndex;

        int predecessorNestCard;
        int lastSeenNestCard;
        bool recalculation;
    };
}

#endif /* defined(__Antz__WorkerRole__) */
