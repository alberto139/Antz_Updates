#include "CommTestRole.h"

using namespace Antz;

CommTestRole::CommTestRole(SmartBot& _robot)
: Role(_robot)
{
    robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_WORK;
}

int CommTestRole::makeStep()
{
    Display& display = robot.display;
    bool wait = true;  // a flag indicating whether there're more signals to be heard
    int roleDecision = NO_SWITCH;
    
    for (int i = 0; i < 6; i++)
        receivedArray[i] = false;

    while (wait)
        wait = receiveSignal(roleDecision);
    robot.wipingNeighborsTimer--;
  
    if (!robot.recver.canHearSignal())
        sendSignal();


    return roleDecision;
}

/* receiveSignal -- receive signals from all the receivers */
bool CommTestRole::receiveSignal(int& roleDecision)
{
    bool received = false;
    int idx[6] = { IDX_FRONT, IDX_LFRONT, IDX_RFRONT, IDX_LREAR, IDX_RREAR, IDX_REAR };

    for (int i = 0; i < 6; i++) // poll from 6 receivers
    {
        uint32_t number;
        if (robot.recver.canHearSignal(idx[i]))
        {
            Serial.print("Receiving from sensor : ");
            Serial.print(idx[i]);
            Serial.print(" ");
            if (receivedArray[idx[i]])
                Serial.println("ABORTED");
            else
            {
                received = true;
                receivedArray[idx[i]] = true;
                uint32_t receivedSignal; // to store the 32-bit signal 
                if (robot.recver.recvFrom(idx[i], &receivedSignal))
                {
                    Neighbor* currentN = new Neighbor(receivedSignal);
                    // ---------------------------------- debug printing
                    Serial.print("Neighbor (");
                    Serial.print(robot.isNeighborValid(*currentN));
                    Serial.print("): ");
                    Serial.print(currentN->id);
                    Serial.print("    ");
                    Serial.print(currentN->curFood);
                    Serial.print("    ");
                    Serial.println(currentN->curNest);
                    if (!robot.isNeighborValid(*currentN))
                    {
                        Serial.print("curFood: ");
                        Serial.print(robot.curFood);
                        Serial.print(", curNest: ");
                        Serial.println(robot.curNest);
                    }
                    // ---------------------------------- end of debug printing

                    if (robot.isNeighborValid(*currentN))
                        robot.registerRobotSignal(*currentN, idx[i]);
                    else
                        delete currentN;
                }
            }
        }
    }

    if (robot.wipingNeighborsTimer == 0)
    {
        robot.formNeighborhood();
        int num = robot.countNeighbors();
        robot.display.number(true, num);
        robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_WORK;
        robot.wipeNeighbors();
    }

    return received;
}
