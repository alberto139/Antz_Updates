#include "LineRole.h"

using namespace Antz;

LineRole::LineRole(SmartBot& _robot)
: Role(_robot),
target(TARGET_NEST),
curNumber(0x00),
numberTimer(0),
maxSignal(0xFF),
signalIndex(6),
maxNumber(0x00),
randomMoveTimer(0),
movePhase(0),
randomCircleCnt(0),
recalculation(false)

{
    robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_WORK;
}

int LineRole::makeStep()
{
    Display& display = robot.display;
    //display.goingTowardsNest(); // yellow LED is turned on when the robot starts walking from food to nest

    signalIndex = 6;
    int roleDecision = NO_SWITCH;
    bool wait = true;
    display.listeningForSignals();
    while (wait)
        wait = receiveSignal(roleDecision);      
    robot.wipingNeighborsTimer--;

    if (!robot.recver.canHearSignal())
    {
        display.sendingSignal(); // when red LED turns off and green turns on, the robot starts sending the signal
        sendSignal();
    }
 
    if (robot.wipingNeighborsTimer == 0)
    {
        //display.number(true, predecessorId);
        robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_WORK;
        robot.minNest = NO_SIGNAL;
        if (predecessorId != lastSeenId)
            makeMovement();
        
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

    for (int i = 2; i < 5; i++) // poll only from 3 rear receivers
    {
        uint32_t number;
        if (robot.recver.recvFrom(i, &number))
        {
            received = true;
            signalIndex = i;

            Neighbor* currentN = new Neighbor(number);
            if (robot.isNeighborValid(*currentN))
            {
                robot.registerRobotSignal(*currentN, i);
                robot.minNest = min(currentN->curNest, robot.minNest);
            }
            else
                delete currentN;
        }
    }

    if (robot.wipingNeighborsTimer == 0)
    {
        robot.formNeighborhood();
        if (robot.countNeighbors() == 0)
        {
            predecessorId = lastSeenId;
            
            if(recalculation)
                robot.goBackward(500, false);
            recalculation = !recalculation;
        }
        else
        {
            lastSeenId = robot.minNest;
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
        delay(100);
        robot.goForward(300, false);
    }  
    else
        robot.evasiveAction(); 
}






