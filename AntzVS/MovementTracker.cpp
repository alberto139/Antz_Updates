#include "MovementTracker.h"

using namespace Antz;

MovementTracker::MovementTracker()
{
    movementsCounter = 0;
    for (int i = 0; i < HISTORY_LENGHT; i++)
        movements[i] = mv_unknown;
}