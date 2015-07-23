//
//  SmartBot.cpp
//  Antz
//
//  Created by Eli Goldweber on 7/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "SmartBot.h"
#include "Neighbor.h"

using namespace Antz;

/* SmartBot -- Constructor */
SmartBot::SmartBot(uint32_t robotId):
AntzRobot(robotId),
curFood(0xFF),
curNest(0xFF),
foodTimer(0),
nestTimer(0),
priority(DEFAULT_PRIORITY),

beacon(true), // role flag
Wcount(0), // added

//worker stuff
target(0),
curNumber(0xFFFFFFFF),
numberTimer(0),
minSignal(0xFF),
signalIndex(6),
minNumber(0xFFFFFFFF),
movePhase(0),
noMoveCnt(0),
randomMoveTimer(0)
 {
}

/* setup -- setup routine for SmartBot robot */
void SmartBot::setup() {
    
    
    AntzRobot::setup();
    Serial.begin(9600);
    
    for (int i =0; i<6; i++) {            //Populate the array
        Neighborhood[i] = Neighbor();
    }
}

/* loop -- loop routine for SmartBot robot */
void SmartBot::loop() {
    
    if(beacon){
        beaconLoop();
        
    }else{
       // workerLoop();
        Serial.println("I AM A WORKER");
        receiveSignalWorker();
    }
  
}

void SmartBot::workerLoop(){
    Serial.println("I AM A WORKER");
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
    receiveSignalWorker();
    
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



}

void SmartBot::beaconLoop(){
    Serial.println("I AM A BEACON");
    //Serial.println("SmartBot::loop() ");
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
         * We see two numbers flashing on a SmartBot's display. One flashes for a
         * relatively longer duration while the other flashes for a fraction of a
         * moment. Number flashing for the longer duration is the number of SmartBot
         * from food whereas the number which appears for a very short time is the
         * number from nest
         */
        display.number(true, curFood);
        delay(100);
        display.number(true, curNest);
        /* Display self position and forward the message to other SmartBots and workers */
        sendSignal();
    }
    else if (priority >= 5)
        priority -= 5;
}

///--------Worker Methods----------

/* receiveSignalWorker -- receive signals from all the receivers */
bool SmartBot::receiveSignalWorker() {
    Serial.println("I AM LOOKING GOT DAMN IT");
    int neighborCount = 0;
    if (millis() - numberTimer > 5000) {
        curNumber = 0xFFFFFFFF;
        numberTimer = millis();
    }
    bool received = false;
    int idx[6] = {IDX_FRONT, IDX_LFRONT, IDX_RFRONT, IDX_LREAR, IDX_RREAR, IDX_REAR};
    for (int i = 0; i < 6; ++i) { // poll from 6 receivers
        uint32_t number;
        if (recver.recvFrom(idx[i], &number)) {
            received = true;
            uint8_t cardinality = target == 0 ? number : (number >> 8);
            
            if (cardinality == 1) {
                Serial.println("BLAH");
                target = 1 - target;
                received = false;
                break;
            }
            else if (cardinality > 0 && cardinality < minSignal) {
                minSignal = cardinality;
                signalIndex = idx[i];
                minNumber = number;
            }

            Neighbor currentN(number);
            Serial.print("Neighbor id: ");
            Serial.println(currentN.id);
            Serial.println(number, BIN);
            Serial.print("\n");
            
            if(Neighborhood[(i+1)%6].id != currentN.id && Neighborhood[((i+6)-1)%6].id != currentN.id
               && Neighborhood[i].id == -1){
                
                if(currentN.id <= 10){ // workaround for random 32 bit numbers resulting in id of 12+
                    Neighborhood[i] = currentN;
                }
            }
            
        }//END if stmt (recver...)
        
    }// END for loop
    
    
    
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
    
    if(Wcount > 3){      // 10 might need to be changed
        //---***---Before Wiping we need to see if the role should be changed---***---///
        
        if(neighborCount <= 1){ //transition
            Serial.print("\n I might need a change.... \n");
            
            int rando = random(42) * 5;
            Serial.println(rando);
            delay (rando);
            
            // check again to make sure I can leave -- still needs to be implemntesd
            Serial.print("\n I DOOOOO need a change.... \n");
            beacon = true;
        }
        
        //---***----------------------------------------------------------------***---///
        
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
void SmartBot::makeMovement() {
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
void SmartBot::randomWalkReset() {
    movePhase = 0;
}

/* randomWalkGo -- make one random movement */
void SmartBot::randomWalkGo() {
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

/////--------END Worker Methods----------


//   -------- Beacon Methods ----------

/* receiveSignal -- receive signals from all the receivers */
bool SmartBot::receiveSignal() {
    bool received = false;
    unsigned long cur = millis();	//millis() returns the number of milliseconds since the arduino began to run this program
    if (cur - nestTimer > 10000)
        curNest = 0xFF;
    if (cur - foodTimer > 10000)
        curFood = 0xFF;
    
    int neighborCount = 0;          // total number of neighbors after a read
    
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
            }
            
            
            Neighbor currentN(number);
            Serial.print("Neighbor id: ");
            Serial.println(currentN.id);
            Serial.println(number, BIN);
            Serial.print("\n");
            
            if(Neighborhood[(i+1)%6].id != currentN.id && Neighborhood[((i+6)-1)%6].id != currentN.id
               && Neighborhood[i].id == -1){
                
                if(currentN.id <= 10){ // workaround for random 32 bit numbers resulting in id of 12+
                    Neighborhood[i] = currentN;
                }
            }
            
        } // end of if (recver.canHearSignal(i))
        
        
    } // end of for (int i = 0; i < 6; ++i)
    
    //Printf of Neighborhood array
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
    
    // Wiping the neighborhood and populating with standard Neighbors with id -1
    if(Wcount > 10){      // 10 might need to be changed
        //---***---Before Wiping we need to see if the role should be changed---***---///
        
        if(neighborCount >= 3){ //transition
             Serial.print("\n I might need a change.... \n");
            
            int rando = random(42) * 5;
            Serial.println(rando);
            delay (rando);
            
             // check again to make sure I can leave -- still needs to be implemntesd
                Serial.print("\n I DOOOOO need a change.... \n");
            beacon = false;
        }
        
        //---***----------------------------------------------------------------***---///
        
        neighborCount = 0;
        Wcount = 0;
        for (int i; i<6; i++){
            Neighborhood[i] = Neighbor();
        }
        Serial.println("-----------------------------------------");
    }
    
    Wcount++;
    
    
    if (minNest < (uint16_t)0xFF && minNest + 1 <= curNest) {
        curNest = minNest + 1;
        nestTimer = millis();
    }
    if (minFood < (uint16_t)0xFF && minFood + 1 <= curFood) {
        curFood = minFood + 1;
        foodTimer = millis();
    }
    return received;
}


/* sendSignal -- Send signals from all the senders */
void SmartBot::sendSignal() {
    uint32_t myNumber = 0;
    
    //Serial.println(myNumber, BIN);
    myNumber |= (identifier << 16);
    myNumber |= ( curFood << 8);
    myNumber |= curNest;
    
    /* message can be visualized as follows:
     *		|----------------|--------|--------|
     *		       16bits       8bits    8bits
     *		  identifier       curFood  curNest
     */
    sender.send(myNumber, 500);
}


/// --------- END Beacon Methods
