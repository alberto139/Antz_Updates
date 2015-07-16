//
//  Tester.cpp
//  Antz
//
//  Created by Zhi Xing on 5/3/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "Tester.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
// Contructor
Tester::Tester(uint32_t robotId): AntzRobot(robotId) {
}

////////////////////////////////////////////////////////////////
// Setup
void Tester::setup() {
    AntzRobot::setup();
    Serial.begin(9600);
}

////////////////////////////////////////////////////////////////
// print out the probabilities in the Bayesian model
void Tester::bayesPrint(const char *description) {
    Serial.print(description);
    Serial.print(": [");
    for (int i = 0; i < 6; ++i) {
        Serial.print(" ");
        Serial.print(condProb[i]);
    }
    Serial.println(" ]");
}

////////////////////////////////////////////////////////////////
// Main loop
void Tester::loop() {
    // test motor asynchronous movements
    /*
    goForward(1000);
    delay(3000);
    goBackward(1000);
    delay(3000);
    turnLeft(60);
    delay(3000);
    turnRight(60);
    delay(3000);
    goForward(10000);
    delay(1000);
    goBackward(10000);
    delay(1000);
    turnLeft(360);
    delay(MOTOR_MS_PER_DEG * 60);
    turnRight(360);
    delay(MOTOR_MS_PER_DEG * 60);
    stopMoving();
    delay(1000);
    */
    
    // test scanner
    /*
    float angle;
    Serial.print(" closes distance = ");
    Serial.print(scanner.scan(&angle));
    Serial.print(", degree = ");
    Serial.println(angle);
    */
    
    // test avoid
    //avoid();
    
    // test bayesian updates
    /*
    bool signals[] = {true, false, false, false, false, false};
    
    Serial.println("=== TEST 1 ===");
    bayesReset();
    bayesPrint("reset");
    bayesUpdate(signals);
    bayesPrint("update 1");
    bayesUpdate(signals);
    bayesPrint("update 2");
    goForward(500, false);
    bayesPrint("go forward");
    goBackward(500, false);
    bayesPrint("go backward");
    turnLeft(60, false);
    bayesPrint("turn left 60");
    turnLeft(120, false);
    bayesPrint("turn left 120");
    turnRight(60, false);
    bayesPrint("turn right 60");
    turnRight(120, false);
    bayesPrint("turn right 120");

    Serial.println("=== TEST 2 ===");
    bayesReset();
    bayesPrint("reset");
    bayesUpdate(signals);
    bayesPrint("update");
    goForward(500);
    delay(600);
    bayesPrint("go forward");
    goBackward(500);
    delay(200);
    bayesUpdate();
    bayesPrint("go backward");
    turnLeft(120);
    delay(MTR_MSPERDEG * 120 + 100);
    bayesPrint("turn left 120");
    turnRight(180);
    delay(MTR_MSPERDEG * 120);
    bayesUpdate();
    bayesPrint("turn right 120");
     */
    
    // test higher speed
    motor.left();
    delay(500);
    motor.stop();
    delay(5000);
}