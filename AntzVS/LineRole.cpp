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
randomCircleCnt(0)

{
    robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_WORK;
}

int LineRole::makeStep()
{
    Display& display = robot.display;
    display.goingTowardsNest(); // yellow LED is turned on when the robot starts walking from food to nest

    //maxSignal = 0;
    signalIndex = 6;
    //maxNumber = 0;
    int roleDecision = NO_SWITCH;
        

    for (int i = 0; i<10; i++)
    {
        if (!robot.recver.canHearSignal())
        {
            //display.sendingSignal(); // when red LED turns off and green turns on, the robot starts sending the signal
            sendSignal();
        }
        receiveSignal(roleDecision);
    }


    if (roleDecision == NO_SWITCH)
    {
        //uint8_t cur = curNumber;
        display.number(true, predecessorId);
        if (predecessorId != lastSeenId)
        //if (maxSignal != 0 && maxSignal >= cur)
        {
            
            //display.number(true, maxSignal);
            //curNumber = maxNumber;
            //numberTimer = millis();
            makeMovement();
            //noMoveCnt = 0;
        }
        
    }

    return roleDecision;
}

/* receiveSignal -- receive signals from all the receivers */
bool LineRole::receiveSignal(int& roleDecision)
{
    /*if (millis() - numberTimer > 5000)
    {
        curNumber = 0;
        numberTimer = millis();
    }*/
    bool received = false;

    for (int i = 2; i < 5; i++) // poll only from 3 rear receivers
    {
        uint32_t number;
        if (robot.recver.recvFrom(i, &number))
        {
            received = true;
            signalIndex = i;
            /*uint8_t cardinality = number;

            if (cardinality == 1)
            {
                target = 1 - target;
                received = false;
                break;
            }
            else if (cardinality > 0 && cardinality > maxSignal)
            {
                maxSignal = cardinality;
                signalIndex = idx[i];
                maxNumber = number;
            }*/

            Neighbor* currentN = new Neighbor(number);
            if (robot.isNeighborValid(*currentN))
                robot.registerRobotSignal(*currentN, i);
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
            robot.goBackward(500, false);
            roleDecision = NO_SWITCH;
        }
        else
        {
            for (int i = 2; i < 5; i++)
                if (robot.neighbors[i] != NULL)
                    lastSeenId = robot.neighbors[i]->id;
        }
        robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_WORK;
        robot.wipeNeighbors();
    }

    robot.wipingNeighborsTimer--;

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
        robot.turnRight(4, false);
        delay(100);
        robot.goForward(300, false);
    }  
    else
        robot.evasiveAction(); 
}






