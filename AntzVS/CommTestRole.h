/*
* Antz
* Antz Framework
* CommTestRole.h
*
* Copyright (c) 2015 All rights reserved.
* License: <#license#>
*/


#ifndef __Antz__CommTestRole__
#define __Antz__CommTestRole__

#include "AntzRobot.h"
#include "Role.h"

namespace Antz
{
    class CommTestRole : public Role
    {
    public:
        CommTestRole(SmartBot& _robot);
        ~CommTestRole() {}

        int getRoleId() { return ROLE_WORKER; };
        int makeStep();

    protected:
        bool receiveSignal(int& roleDecision);

        bool receivedArray[6];
    };
}

#endif /* defined(__Antz__WorkerRole__) */
