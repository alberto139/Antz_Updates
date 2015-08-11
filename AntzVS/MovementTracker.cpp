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
  if(movementsCounter < HISTORY_LENGTH)
    movements[movementsCounter] = movement;
  else
  {
    int overrideMovement = (movementsCounter % HISTORY_LENGTH)-1;
    if(overrideMovement >= 0)
      movements[overrideMovement] = movement;
    else
       movements[HISTORY_LENGTH-1] = movement;
  }  
}

MovementTracker::Movement MovementTracker::getLastMovement()
{
    Movement lastMove = mv_unknown;
    int overrideMovement = (movementsCounter % HISTORY_LENGTH)-1;
    if(overrideMovement >= 0)
      lastMove = movements[overrideMovement];
    else
       lastMove = movements[HISTORY_LENGTH-1];
    
    return lastMove;
  
}

MovementTracker::Movement MovementTracker::getMovementDecision()
{

  return getOppMovement(getLastMovement());
}


MovementTracker::Movement MovementTracker::getOppMovement(Movement movement)
{
  switch (movement)
    {
          case mv_forward:
            return mv_backward;
          case mv_backward:
            return mv_forward; 
          case mv_left_forward:
            return mv_right_backward;
          case mv_left_backward:
            return mv_right_forward;
          case mv_right_forward:
            return mv_left_backward;
          case mv_right_backward:
            return mv_left_forward;
          default:
            return mv_unknown; 
    }
}




