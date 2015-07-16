/*
 * Antz
 * Antz Framework
 * Guider.cpp
 *		Definitions of various routines for Guider robot.
 *
 * Copyright: 2015 Ac Zhi Xing. All rights reserved.
 * License: <#license#>
 */

#include "Guider.h"

using namespace Antz;

/* Guider -- Contructor */
Guider::Guider(uint32_t robotId):
  AntzRobot(robotId),
  curFood(0xFF),
  curNest(0xFF),
  foodTimer(0),
  nestTimer(0),
  Neighborhood = {0},
  priority(DEFAULT_PRIORITY) {
}

/* setup -- setup routine for Guider robot */
void Guider::setup() {
    AntzRobot::setup();
}

/* loop -- loop routine for Guider robot */
void Guider::loop() {
  
  //Serial.println("Guider::loop() ");
  //display.number(true, 8);
    stopMoving();
    minFood = 0xFF;			// to store the minimum food cardinality
    minNest = 0xFF;			// to store the minimum nest cardinality
    
    display.blue(false);
    display.yellow(false);
    display.red(true);		// when red LED is on, the robot listens to the signals from surroundings
    display.green(false);
    bool wait = true;		// a flag indicating whether there're more signals to be heard
	/* keep looping until message is heard */
    while (wait || minNest == 0xFF && minFood == 0xFF)
        wait = receiveSignal();
    
    delay(random(priority) * 10);
    
    if (!recver.canHearSignal()) {
        priority = DEFAULT_PRIORITY;
        display.red(false);
        display.green(true);		// when red LED turns off and green turns on, the robot starts sending the signal
	/* 
	 * We see two numbers flashing on a guider's display. One flashes for a 
	 * relatively longer duration while the other flashes for a fraction of a 
	 * moment. Number flashing for the longer duration is the number of guider 
	 * from food whereas the number which appears for a very short time is the 
	 * number from nest
	 */
        display.number(true, curFood);
        delay(100);
        display.number(true, curNest);
	/* Display self position and forward the message to other guiders and workers */
        sendSignal();
    }
    else if (priority >= 5)
        priority -= 5;
}

/* receiveSignal -- receive signals from all the receivers */
bool Guider::receiveSignal() {
    bool received = false;
    unsigned long cur = millis();	//millis() returns the number of milliseconds since the arduino began to run this program
    if (cur - nestTimer > 10000)
        curNest = 0xFF;
    if (cur - foodTimer > 10000)
        curFood = 0xFF;
    
    for (int i = 0; i < 6; ++i) {	// 6 is the number of receiver sensors mounted on the robot
        if (recver.canHearSignal(i)) {
            received = true;
            uint32_t number; // to store the 32-bit signal
            if (recver.recvFrom(i, &number)) {
                uint8_t nest = (uint8_t)(number & 0xFF);
                uint8_t food = (uint8_t)(number >> 8);
                if (nest > 0 && nest < minNest)
                    minNest = nest;
                if (food > 0 && food < minFood)
                    minFood = food;
                //added
                Neighbor x = new Neighbor(number);
                if ( i = 0){
                    if((Neighborhood[5] != NULL || Neighborhood[5].getID == x.getID)&&
                       (Neighborhood[1] != NULL || Neighborhood[1].getID == x.getID)){
                        Neighborhood[0] = x;
                    }
                    
                }
                else if( i = 5){
                    if(Neighborhood[0] != NULL || Neighborhood[0].getID == x.getID)&&
                      (Neighborhood[4] != NULL || Neighborhood[4].getID == x.getID)){
                          Neighborhood[0] = x;
                      }
                }
                else{
                    if(Neighborhood[i+1] != NULL || Neighborhood[i+1].getID == x.getID)&&
                        (Neighborhood[i-1] != NULL || Neighborhood[i-1].getID == x.getID)){
                            Neighborhood[i] = x;
                        }

                }
                
                int neighborCount = 0;
                for(int j = 0; j<6; j++){
                    if(Neighborhood[i] != NULL){
                        neighborCount++;
                        
                    }
                    
                }
                Serial.println("Number Of Neighbors: ");
                Serial.print(neighborCount);

                
                
                ///
            }
        
        
        /*
        Serial.println("Number form sensor: ");
        Serial.print(i);
        Serial.print("  ");
        Serial.print(number, BIN);
        Serial.println();
        */
        }
        
    }
    if (minNest < (uint16_t)0xFF && minNest + 1 <= curNest) {
        curNest = minNest + 1;
        nestTimer = millis();
    }
    if (minFood < (uint16_t)0xFF && minFood + 1 <= curFood) {
        curFood = minFood + 1; 
        //Serial.println("I'm Here. In Guider::receiveSignal() ");
        foodTimer = millis();
    }
    return received;
}

/* sendSignal -- Send signals from all the senders */
void Guider::sendSignal() {
    uint32_t myNumber = 0;

    //Serial.println(myNumber, BIN);		
    //myNumber |= (curFood << 7);	      // Changed form shifting by 8, to shifting by 7 and then 1. To avoid breaking the ID in mynumber
    //myNumber = (myNumber << 1);
    myNumber |= (identifier << 16);
    //myNumber |= ( (uint16_t)curFood << 8);
    myNumber |= ( curFood << 8);
    	
    //Serial.println(myNumber, BIN);		
    myNumber |= curNest;

	/* message can be visualized as follows:
	 *		|----------------|--------|--------|
	 *		       16bits       8bits    8bits
	 *		  identifier       curFood  curNest
	 */
    sender.send(myNumber, 500);
    
    Serial.print("myNumber: ");
    Serial.println(myNumber, BIN);
    Serial.print("ID      : ");
    Serial.println(identifier, BIN);
    Serial.print("curFood : ");
    Serial.println(curFood, BIN);
    Serial.print("curNest : ");
    Serial.println(curNest, BIN);

}
