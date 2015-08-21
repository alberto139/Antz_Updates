#include "WorkerRole.h"

using namespace Antz;

WorkerRole::WorkerRole(SmartBot& _robot)
    : Role(_robot), 
    target(TARGET_FOOD), 
    curNumber(0xFFFFFFFF), 
    numberTimer(0), 
    minSignal(0xFF),
    signalIndex(6),
    minNumber(0xFFFFFFFF),
    randomMoveTimer(0),
    movePhase(0),
    randomCircleCnt(0)
    
{
    robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_WORK;
}

int WorkerRole::makeStep()
{
    Display& display = robot.display;
    if (target == TARGET_NEST)
        display.goingTowardsNest(); // yellow LED is turned on when the robot starts walking from food to nest
    else if (target == TARGET_FOOD)
        display.goingTowardsFood(); // blue LED is turned on when the robot starts walking from nest to food

    minSignal = NO_SIGNAL;
    signalIndex = 6;
    minNumber = 0xFFFFFFFF;
    int roleDecision = NO_SWITCH;
    //sendSignal(); -- for now workers do not send any signal
    
    for(int i = 0; i < NEIGHBORS_COLLECTION_TIME_WORK; i++)
        receiveSignal(roleDecision);

    if (roleDecision == NO_SWITCH)
    {
        uint8_t cur = target == TARGET_NEST ? curNumber : (curNumber >> 8);

        if (minSignal != NO_SIGNAL && minSignal <= cur)
        {
            display.number(true, minSignal);
            //display.number(true,signalIndex);
            curNumber = minNumber;
            numberTimer = millis();
            makeMovement(); //**************** <---
            randomWalkReset();
            noMoveCnt = 0;
        }
        else if (curNumber == 0xFFFFFFFF)
        {
            if (millis() - randomMoveTimer > 1000)
            {
                display.number(false,signalIndex);
                randomWalkGo();//**************** <---
                randomMoveTimer = millis();
            }
        }
    }

    return roleDecision;
}

/* receiveSignal -- receive signals from all the receivers */
bool WorkerRole::receiveSignal(int& roleDecision)
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
       if (robot.recver.canHearSignal(idx[i]))
        {
        Serial.print("Receiving from sensor : ");
        Serial.println(idx[i]);
        if (robot.recver.recvFrom(idx[i], &number))
        {
            received = true;

            uint8_t cardinality = target == TARGET_NEST ? number : (number >> 8);

            if (cardinality == 1)
            {
                target = 1 - target;
                received = false;
                break;
            }
            else if (cardinality > 0 && cardinality < minSignal)
            {
                minSignal = cardinality;
                signalIndex = idx[i];
                minNumber = number;
                
            }

            Neighbor* currentN = new Neighbor(number);
            if (robot.isNeighborValid(*currentN))
            {
                //robot.minFood = min(currentN->curFood, robot.minFood);
                //robot.minNest = min(currentN->curNest, robot.minNest);
                robot.registerRobotSignal(*currentN, idx[i]);
            }
            else
                delete currentN;
        }
        }
    }

    if (robot.wipingNeighborsTimer == 0)
    {
        robot.formNeighborhood();
        int neighborsCount = robot.countNeighbors();
        if (neighborsCount <= 1 && target == TARGET_FOOD)
        {
          Serial.println("role descision");
            roleDecision = NO_SWITCH;
        }
        if (neighborsCount >= 1)
        {
         
            Neighbor* toFollow = robot.getLowestCardNeighbor(target);
            //signalIndex = toFollow->mostlySeenFrom();
        }
        robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_WORK;
        robot.wipeNeighbors();
    }

    robot.wipingNeighborsTimer--;

    return received;
}

/* makeMovement -- make a movement based on current signal index */
void WorkerRole::makeMovement()
{
    switch (signalIndex)
    {
          case IDX_FRONT:
            robot.turnLeft(40);
            break;
        case IDX_REAR:
            robot.turnLeft(180);
            break;
        case IDX_LFRONT:
            //robot.turnLeft(120);
            robot.turnLeft(60);
            break;
        case IDX_LREAR:
            robot.turnLeft(120);
            break;
        case IDX_RFRONT:
            // ---
            robot.turnRight(60,false);
            float angle;
            while (robot.scanner.scan(&angle) > 25)
                robot.goForward(200,false);
            robot.turnLeft(60,false);
            // --- END
             if (!robot.blocked())
            {
                robot.goForward(200,false);
                if (!robot.blocked())
                  robot.goForward(200,false);
                else
                  robot.evasiveAction();
            }
            else
                robot.evasiveAction();
            break;
        case IDX_RREAR:
            robot.turnRight(60);
            break;
      
//        case IDX_FRONT:
//            if (!robot.blocked())
//            {
//                robot.goForward(200);
//                if (!robot.blocked())
//                  robot.goForward(200);
//                else
//                  robot.evasiveAction();
//            }
//            else
//                robot.evasiveAction();
//            break;
//        case IDX_REAR:
//            robot.turnLeft(180);
//            break;
//        case IDX_LFRONT:
//            robot.turnLeft(60);
//            break;
//        case IDX_LREAR:
//            robot.turnLeft(120);
//            break;
//        case IDX_RFRONT:
//            robot.turnRight(60);
//            break;
//        case IDX_RREAR:
//            robot.turnRight(120);
//            break;
    }
}

/* randomWalkReset -- reset random walk state */
void WorkerRole::randomWalkReset()
{
    movePhase = 0;
}

/* randomWalkGo -- make one random movement */
void WorkerRole::randomWalkGo()
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
            
            if(randomCircleCnt == 0)
            {
                robot.turnRight(60, false);
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
    if(randomCircleCnt == 0)
        movePhase++;
}



