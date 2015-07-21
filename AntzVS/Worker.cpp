/*
* Antz
* Antz Framework
* Worker.cpp
*		Defines routines related workers' movements and communications
* Copyright (c) 2015 Zhi Xing. All rights reserved.
* License: <#license#>
*/

#include "Worker.h"
#include "Neighbor.h"

using namespace Antz;

/* Worker -- Contructor */
Worker::Worker(uint32_t robotId):
AntzRobot(robotId),
target(0),
curNumber(0xFFFFFFFF),
numberTimer(0),
minSignal(0xFF),
signalIndex(6),
minNumber(0xFFFFFFFF),
movePhase(0),
noMoveCnt(0),
Wcount(0), // added in the constructor //counter for wipe 
randomMoveTimer(0){
}

/* setup -- setup routine for Worker robot */
void Worker::setup() {
    AntzRobot::setup();
    Serial.begin(9600);
    
    for (int i =0; i<6; i++) {            //Populate the array in setup
          Neighborhood[i] = Neighbor();
        }
        
    Serial.println("In Worker setup------------------------------------------------------");
        
    
}

/* loop -- loop routine for Worker robot */
void Worker::loop() {
  Serial.println("In Worker loop------------------------------------------------------");
    display.red(false);
    display.green(false);
    if (target == 0) { // i'm going towards nest
        display.blue(false);
        display.yellow(true);		// yellow LED is turned on when the robot starts walking from food to nest
    }
    else if (target == 1) {
        display.blue(true);			// blue LED is turned on when the robot starts walking from nest to food
        display.yellow(false);
    }
    
    minSignal = 0xFF;
    signalIndex = 6;
    minNumber = 0xFFFFFFFF;
    receiveSignal();
    
    uint8_t cur = target == 0 ? curNumber : (curNumber >> 8);
    
    if (minSignal != 0xFF && minSignal <= cur) {
        display.number(true, minSignal);
        curNumber = minNumber;
        numberTimer = millis();
        makeMovement();
        randomWalkReset();
        noMoveCnt = 0;
    }
    else if (curNumber == 0xFFFFFFFF) {
        if (millis() - randomMoveTimer > 1000) {
            randomWalkGo();
            randomMoveTimer = millis();
        }
    }
    
  // if( == 1 ){
     
}

/* receiveSignal -- receive signals from all the receivers */
bool Worker::receiveSignal() {
  
    int neighborCount = 0;          // total number of neighbors after a read
    
    if (millis() - numberTimer > 5000) {
        curNumber = 0xFFFFFFFF;
        numberTimer = millis();
    }
    bool received = false;
    int idx[6] = {IDX_FRONT, IDX_LFRONT, IDX_RFRONT, IDX_LREAR, IDX_RREAR, IDX_REAR};
    int beacCount = 0;
    
    for (int i = 0; i < 6; ++i) { // poll from 6 receivers
        uint32_t number;
        if (recver.recvFrom(idx[i], &number)) {
            received = true;
            
            uint8_t cardinality = target == 0 ? number : (number >> 8);
            
            if (cardinality == 1) {
                target = 1 - target;
                received = false;
                break;
            }
            else if (cardinality > 0 && cardinality < minSignal) {
                minSignal = cardinality;
                signalIndex = idx[i];
                beacCount ++;         //increment the number of beacons seen 
                minNumber = number;
            }
        } 
        
        Neighbor currentN(number);
        
        if(Neighborhood[(i+1)%6].id != currentN.id && Neighborhood[((i+6)-1)%6].id != currentN.id
               && Neighborhood[i].id == -1){
                
                    if(currentN.id < 15){ // debugging for only id's of '0'
                        Neighborhood[i] = currentN;
                    }
                }
    }
    ///////////////////////////////////////////////////////////////////////////////////
    
    
    Serial.println("Neighborhood array");
              for (int i; i<6; i++){
                Serial.print(Neighborhood[i].id);
                Serial.print("  ");
                if (Neighborhood[i].id != -1){
                  neighborCount++;
                }
              }
                Serial.println("");
                Serial.print("# Neighbors: ");
                Serial.println(neighborCount);
                //End of Printf of Neighborhood array
                
                // Wiping the neighborhood and populating with standard Neighbors with id -1
                if(Wcount > 10){      // 10 might need to be changed
                
                if(neighborCount == 1){
                  Serial.println("I am going to Transition role");
                  

                  transition();
                  
                }

                  neighborCount = 0;
                  Wcount = 0;
                  for (int i; i<6; i++){
                    Neighborhood[i] = Neighbor();
                  }
                  Serial.println("-----------------------------------------");
                }
                
                Wcount++;
    
    return received;
}

/* makeMovement -- make a movement based on current signal index */
void Worker::makeMovement() {
    switch (signalIndex) {
        case IDX_FRONT:
            if (avoid() == false)
                goForward(500);
            break;
        case IDX_REAR:
            turnLeft(180);
            break;
        case IDX_LFRONT:
            turnLeft(60);
            break;
        case IDX_LREAR:
            turnLeft(120);
            break;
        case IDX_RFRONT:
            turnRight(60);
            break;
        case IDX_RREAR:
            turnRight(120);
            break;
    }
}

/* randomWalkReset -- reset random walk state */
void Worker::randomWalkReset() {
    movePhase = 0;
}

/* randomWalkGo -- make one random movement */
void Worker::randomWalkGo() {
    switch (movePhase) {
        case 0:
            turnLeft(30);
            break;
        case 1:
            turnRight(30);
            break;
        case 2:
            turnRight(60, false);
            if (!avoid())				// if there is no obstacle
                goForward(500);
            break;
        default:
            turnLeft(60, false);
            int cnt = movePhase;
            while (cnt-- > 0) {
                if (!avoid())			// if there is no obstacle
                    goForward(500);
            }
            break;
    }
    ++movePhase;
}

void Worker::transition(){
  AntzRobot *antz = AntzRobot::createAntzRobot("Guider", identifier);
  antz->setup();
  
  while(true){
    antz->loop();
  }
}
