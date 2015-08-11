#include "MovementTracker.h"

using namespace Antz;

MovementTracker::MovementTracker()
{
    movementsCounter = 0;
    for (int i = 0; i < HISTORY_LENGTH; i++)
        movements[i] = mv_unknown;
}

void MovementTracker::addMovement(Movement movement)
{
    movements[movementsCounter % HISTORY_LENGTH] = movement;
    movementsCounter++;
}

MovementTracker::Movement MovementTracker::getLastMovement()
{
    return movements[(movementsCounter - 1) % HISTORY_LENGTH];
  
}

MovementTracker::Movement MovementTracker::getMovementDecision()
{
    return mv_forward;
}


MovementTracker::Movement MovementTracker::getOppMovement(Movement movement)
{

//          enum Movement { mv_forward, mv_backward, 
//                        mv_left_forward, mv_left_backward, 
//                        mv_right_forward, mv_right_backward, 
//                        mv_unknown };
  switch (movement)
    {
          case mv_forward:
            return mv_backward;
          case mv_backward:
            return mv_forward;
            
    }
  
}




