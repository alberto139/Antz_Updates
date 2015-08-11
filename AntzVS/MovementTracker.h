/*
* Antz
* Antz Framework
* MovementTracker.h
*
* Copyright: 2015. All rights reserved.
* License: <#license#>
*/


#ifndef __Antz__MovementTracker__
#define __Antz__MovementTracker__

#define HISTORY_LENGHT 10

namespace Antz
{
    class MovementTracker
    {
    public:
        enum Movement { mv_forward, mv_backward, 
                        mv_left_forward, mv_left_backward, 
                        mv_right_forward, mv_right_backward, 
                        mv_unknown };
        MovementTracker();

        void addMovement(Movement movement);
        Movement getLastMovement();
        Movement getMovementDecision();

    private:
        Movement movements[HISTORY_LENGHT];
        int movementsCounter;
    };
}

#endif /* defined(__Antz__MovementTracker__) */
