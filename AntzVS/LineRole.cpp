#include "LineRole.h"

using namespace Antz;

LineRole::LineRole(SmartBot& _robot)
: Role(_robot),
target(TARGET_NEST),
numberTimer(0),
signalIndex(6),
predecessorNestCard(NO_SIGNAL),
lastSeenNestCard(NO_SIGNAL),
recalculation(false)

{
    robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_WORK;
}

int LineRole::makeStep()
{

  
    int roleDecision = NO_SWITCH;
    robot.wipingNeighborsTimer--;

    Display& display = robot.display;
    display.listeningForSignals(); // red on, green off
    bool wait = true;
    while (wait)
        wait = receiveSignal(roleDecision);

    if (!robot.recver.canHearSignal() && robot.wipingNeighborsTimer % 3 == 0)
    {
        display.sendingSignal(); // red off, green on
        sendSignal();
    }
    display.number(true, predecessorNestCard);
    if (robot.wipingNeighborsTimer == 0)
    {
        if (predecessorNestCard != lastSeenNestCard)
            makeMovement();
        signalIndex = 6;
        robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_LINE;
        robot.minNest = NO_SIGNAL;
    }

    return roleDecision;
}

/* receiveSignal -- receive signals from all the receivers */
bool LineRole::receiveSignal(int& roleDecision)
{
    if (millis() - numberTimer > 5000)
    {
        robot.curNest = NO_SIGNAL;
        numberTimer = millis();
    }
    bool received = false;

    int receivers[3] = { IDX_LREAR, IDX_REAR, IDX_RREAR };
    for (int i = 0; i < 3; i++) // poll only from 3 rear receivers
    {
      
        uint32_t number;
        if (robot.recver.recvFrom(receivers[i], &number))
        {
            received = true;
            signalIndex = receivers[i];
            Neighbor* currentN = new Neighbor(number);
            if (robot.isNeighborValid(*currentN))
            {
                robot.registerRobotSignal(*currentN, receivers[i]);
                robot.minNest = min(currentN->curNest, robot.minNest);
            }
            else
                delete currentN;
        }
        
    }

    if (robot.wipingNeighborsTimer == 0)
    {
        received = false; // to prevent forming neighborhood again in the same iteration (see while loop in makeStep)
        robot.formNeighborhood();
        if (robot.countNeighbors() == 0)
        {
            if (recalculation)
            {
                predecessorNestCard = lastSeenNestCard;
                robot.goBackward(500, false);
                
            }
            recalculation = !recalculation;
        }
        else
        {
            lastSeenNestCard = robot.minNest;
            bool anythingMoving = false;
            for (int i = 0; i < 6; i++)
                anythingMoving |= robot.neighbors[i] != NULL && robot.neighbors[i]->role == ROLE_WORKER;
            if (anythingMoving && lastSeenNestCard > 2)
                predecessorNestCard = NO_SIGNAL;
            recalculation = false;
            
        }
        robot.wipeNeighbors();
    }

    if (robot.minNest != NO_SIGNAL && robot.minNest + 1 <= robot.curNest)
    {
        robot.curNest = robot.minNest + 1;
        numberTimer = millis();
    }

    return received;
}

/* makeMovement -- make a movement based on current signal index */
void LineRole::makeMovement()
{
    switch (signalIndex)
    {
        case IDX_FRONT:
            break;
        case IDX_REAR:
            forwardStep();
            break;
        case IDX_LFRONT:
            break;
        case IDX_LREAR:
            forwardStep();
            break;
        case IDX_RFRONT:
            break;
        case IDX_RREAR:
            forwardStep();
            break;
    }
}

void LineRole::forwardStep()
{
    if (!robot.blocked())
    {
        //robot.turnRight(4, false);
        //delay(100);
        robot.goForward(300, false);
    }  
    else
        robot.evasiveAction(); 
}


int LineRole::getRoleId()
{
  if(predecessorNestCard == lastSeenNestCard)
    return ROLE_GUIDER;
  else
    return ROLE_WORKER;

}



