#include "GuiderRole.h"

using namespace Antz;

GuiderRole::GuiderRole(SmartBot& _robot)
    : Role(_robot), recalculation(false), foodTimer(0), nestTimer(0), priority(DEFAULT_PRIORITY)
{
    robot.stopMoving();
    robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_GUID;
}

void GuiderRole::makeTurn()
{
      int turnDegrees = random(15);
      
      if (turnDegrees%2 == 0) 
       robot.turnLeft(turnDegrees);
      else
       robot.turnRight(turnDegrees);
}

int GuiderRole::makeStep()
{
    if(robot.wipingNeighborsTimer % 3 == 0)
        makeTurn();

  
//    Reposcount ++;
//    if(Reposcount > NEIGHBORS_COLLECTION_TIME_GUID * 5)
//    {
//      Serial.println("Turning...");
//      makeTurn();
//      Reposcount = 0;
//    }
    robot.minFood = NO_SIGNAL;			// to store the minimum food cardinality
    robot.minNest = NO_SIGNAL;			// to store the minimum nest cardinality

    Display& display = robot.display;
    display.listeningForSignals(); // when red LED is on, the robot listens to the signals from surroundings

    bool wait = true;	// a flag indicating whether there're more signals to be heard
    int roleDecision = NO_SWITCH;
    /* keep looping until message is heard */

    if(robot.curFood == NO_SIGNAL) //if the food is seen, only send signal
    {
    while (roleDecision == NO_SWITCH && (wait || robot.minNest == NO_SIGNAL && robot.minFood == NO_SIGNAL))
        wait = receiveSignal(roleDecision);


    robot.wipingNeighborsTimer--;
    }
    if (roleDecision == NO_SWITCH)
    {
        delay(random(priority) * 10);

        if (!robot.recver.canHearSignal())
        {
            priority = DEFAULT_PRIORITY;
            display.sendingSignal(); // when red LED turns off and green turns on, the robot starts sending the signal
            /*
            * We see two numbers flashing on a guider's display. One flashes for a
            * relatively longer duration while the other flashes for a fraction of a
            * moment. Number flashing for the longer duration is the number of guider
            * from food whereas the number which appears for a very short time is the
            * number from nest
            */
            display.number(true, robot.curFood);
            delay(100);
            display.number(true, robot.curNest);
            /* Display self position and forward the message to other guiders and workers */
            sendSignal();
        }//test
         else if (priority >= 5) 
            priority -= 5;
    }
    return roleDecision;
}

/* receiveSignal -- receive signals from all the receivers */
bool GuiderRole::receiveSignal(int& roleDecision)
{
    bool received = false;
    unsigned long cur = millis();	//millis() returns the number of milliseconds since the arduino began to run this program
    if (cur - nestTimer > 10000)
        robot.curNest = NO_SIGNAL;
    if (cur - foodTimer > 10000)
        robot.curFood = NO_SIGNAL;


    for (int i = 0; i < 6; i++) // 6 is the number of receiver sensors mounted on the robot
    {
        if (robot.recver.canHearSignal(i))
        {
            received = true;
            uint32_t receivedSignal; // to store the 32-bit signal 
            if (robot.recver.recvFrom(i, &receivedSignal))
            {	
                Neighbor* currentN = new Neighbor(receivedSignal);
                
                /*Serial.print("Received: ");
                Serial.println(currentN->id);
                Serial.println(currentN->orgSignal, BIN);*/
                if (robot.isNeighborValid(*currentN))
                {
                    //Serial.println("validated");
                    robot.minFood = min(currentN->curFood, robot.minFood);
                    robot.minNest = min(currentN->curNest, robot.minNest);
                    robot.registerRobotSignal(*currentN, i);
                }
                else
                    delete currentN;
            }
        } 
    }

    if (robot.wipingNeighborsTimer == 0) // wiping neighbors when timer reaches 0
    {
        
        robot.formNeighborhood();
        if (robot.countNeighbors() >= 3)
        {
          
            if (!recalculation)
            {
                recalculation = true;
                Serial.println("Recalculating...");
                delay(random(1000));
                robot.wipingNeighborsTimer =  NEIGHBORS_COLLECTION_TIME_GUID; // same as 1st check
            }
            else
                roleDecision = SWITCH_ROLE;
           
        }
        else
        {
            recalculation = false;
            robot.wipingNeighborsTimer = NEIGHBORS_COLLECTION_TIME_GUID;
        }
        robot.wipeNeighbors();
        //Serial.println("-----------------------------------------");
    }

    //robot.wipingNeighborsTimer--;


    if (robot.minNest < (uint16_t)0xFF && robot.minNest + 1 <= robot.curNest)
    {
        robot.curNest = robot.minNest + 1;
        nestTimer = millis();
    }
    if (robot.minFood < (uint16_t)0xFF && robot.minFood + 1 <= robot.curFood)
    {
        robot.curFood = robot.minFood + 1;
        foodTimer = millis();
    }
    return received;
}
