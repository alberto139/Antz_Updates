/*
* Antz
* Antz Framework
* Role.h
*
* Copyright (c) 2015. All rights reserved.
* License: <#license#>
*/


#ifndef __Antz__Role__
#define __Antz__Role__

#include "SmartBot.h"

#define TARGET_NEST 0
#define TARGET_FOOD 1

#define ROLE_WORKER 0
#define ROLE_GUIDER 1

#define SWITCH_ROLE 0
#define NO_SWITCH	1

namespace Antz
{
    class Role
    {
    public:
        Role(SmartBot& _robot) : robot(_robot) {}
        virtual ~Role() {}
        virtual int getRoleId() = 0;
        virtual int makeStep() = 0;

    protected:
        virtual bool receiveSignal(int& roleDecision) = 0;
        void sendSignal();

        SmartBot& robot;
    };

}

#endif
