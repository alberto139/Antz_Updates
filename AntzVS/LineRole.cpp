#include "LineRole.h"

using namespace Antz;

LineRole::LineRole(SmartBot& _robot)
: Role(_robot),
target(TARGET_NEST),
curNumber(0xFFFFFFFF),
numberTimer(0),
maxSignal(0xFF),
signalIndex(6),
maxNumber(0xFFFFFFFF),
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

    maxSignal = NO_SIGNAL;
    signalIndex = 6;
    maxNumber = 0xFFFFFFFF;
    int roleDecision = NO_SWITCH;
    //sendSignal(); -- for now workers do not send any signal

    for (int i = 0; i<5; i++)
        receiveSignal(roleDecision);

    if (roleDecision == NO_SWITCH)
    {
        uint8_t cur = curNumber;

        if (maxSignal != NO_SIGNAL && maxSignal >= cur)
        {
            display.number(true, maxSignal);
            curNumber = maxNumber;
            numberTimer = millis();
            makeMovement();
            randomWalkReset();
            noMoveCnt = 0;
        }
        else if (curNumber == 0xFFFFFFFF)
        {
            if (millis() - randomMoveTimer > 1000)
            {

                randomWalkGo();
                randomMoveTimer = millis();
            }
        }
    }

    return roleDecision;
}

/* receiveSignal -- receive signals from all the receivers */
bool LineRole::receiveSignal(int& roleDecision)
{
    if (millis() - numberTimer > 5000)
    {
        curNumber = 0xFFFFFFFF;
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
            robot.turnLeft(180);
            robot.goForward(200);
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
        robot.turnLeft(60);
        break;
    case IDX_REAR:
        if (!robot.blocked())
            robot.goForward(400);
        else
            robot.evasiveAction();
        break;
    case IDX_LFRONT:
        if (!robot.blocked())
            robot.goForward(400);
        else
            robot.evasiveAction();
        break;
    case IDX_LREAR:
        robot.turnLeft(90);
        robot.goForward(800);
        robot.turnRight(90);
        break;
    case IDX_RFRONT:
        if (!robot.blocked())
            robot.goForward(400);
        else
            robot.evasiveAction();
        break;
        break;
    case IDX_RREAR:
        robot.turnRight(90);
        robot.goForward(800);
        robot.turnLeft(90);
        break;
    }
}

/* randomWalkReset -- reset random walk state */
void LineRole::randomWalkReset()
{
    movePhase = 0;
}

/* randomWalkGo -- make one random movement */
void LineRole::randomWalkGo()
{
    switch (movePhase)
    {
    case 0:
        robot.turnLeft(30);
        break;
    case 1:
        robot.turnRight(30);
        break;
    case 2:
        robot.turnRight(60, false);
        if (!robot.blocked())				// if there is no obstacle
            robot.goForward(400);
        else
            robot.evasiveAction();
        break;
    default:

        if (randomCircleCnt == 0)
        {
            robot.turnLeft(60, false);
            randomCircleCnt = movePhase;
        }
        if (randomCircleCnt-- > 0)
        {
            if (!robot.blocked())
            {
                robot.goForward(200);
                if (!robot.blocked())
                    robot.goForward(200);
                else
                    robot.evasiveAction();
            }
            else
                robot.evasiveAction();

        }
        //movePhase = -1;
        break;
    }
    if (randomCircleCnt == 0)
        movePhase++;
}