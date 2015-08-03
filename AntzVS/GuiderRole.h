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

#include "Role.h"
#include "Display.h"

namespace Antz 
{
    class GuiderRole : public Role
    {
    public:
        GuiderRole(SmartBot& _robot);
        ~GuiderRole() {}

        int getRoleId() { return ROLE_GUIDER; };
        int makeStep();
        int Reposcount = 0;

    private:
        bool receiveSignal(int& roleDecision);

        bool recalculation;
         // Maybe we should change the variable names below
        uint64_t foodTimer;
        uint64_t nestTimer;
        uint8_t priority;
    
    };
}

#endif /* defined(__Antz__GuiderRole__) */
