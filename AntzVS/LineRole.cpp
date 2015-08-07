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

    maxSignal = 0;
    signalIndex = 6;
    maxNumber = 0;
    int roleDecision = NO_SWITCH;
    //sendSignal(); -- for now workers do not send any signal

    for (int i = 0; i<10; i++)
        receiveSignal(roleDecision);

    if (roleDecision == NO_SWITCH)
    {
        uint8_t cur = curNumber;

        if (maxSignal != 0 && maxSignal >= cur)
        {
            display.number(true, maxSignal);
            curNumber = maxNumber;
            numberTimer = millis();
            makeMovement();
            noMoveCnt = 0;
        }
        
    }

    return roleDecision;
}

/* receiveSignal -- receive signals from all the receivers */
bool LineRole::receiveSignal(int& roleDecision)
{
    if (millis() - numberTimer > 5000)
    {
        curNumber = 0;
        numberTimer = millis();
    }
    bool received = false;
    int idx[6] = { IDX_FRONT, IDX_LFRONT, IDX_RFRONT, IDX_LREAR, IDX_RREAR, IDX_REAR };

    for (int i = 0; i < 6; i++) // poll from 6 receivers
    {
        uint32_t number;
        if (robot.recver.recvFrom(idx[i], &number))
        {
            received = true;

            uint8_t cardinality = number;

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
            }

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
            robot.turnLeft(180,false);
            delay(500);
            robot.goForward(550,false);
            delay(500);
            robot.turnRight(25,false);
            roleDecision = SWITCH_ROLE;
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
        forwardStep();
        break;
    case IDX_REAR:
        forwardStep();
        break;
    case IDX_LFRONT:
        forwardStep();
        break;
    case IDX_LREAR:
        robot.goForward(300, false);
        robot.turnLeft(90, false);
        robot.goForward(800, false);
        robot.turnRight(90, false);
        break;
    case IDX_RFRONT:
        forwardStep();
        break;
    case IDX_RREAR:
        robot.goForward(300, false);
        robot.turnRight(90, false);
        robot.goForward(800, false);
        robot.turnLeft(90, false);
        break;
    }
}

void LineRole::forwardStep()
{
 if (!robot.blocked())
        {
            robot.turnRight(4,false);
            delay(100);
            robot.goForward(400,false);
        }  
        else
            robot.evasiveAction(); 
}






