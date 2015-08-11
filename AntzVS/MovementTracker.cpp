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




