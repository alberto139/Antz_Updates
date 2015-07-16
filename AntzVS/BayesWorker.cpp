//
//  BayesWorker.cpp
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "BayesWorker.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
// Contructor
BayesWorker::BayesWorker(uint32_t robotId): AntzRobot(robotId), target(0), curSource(0xFFFFFFFF), sourceTime(0) {
}

////////////////////////////////////////////////////////////////
// Setup
void BayesWorker::setup() {
    AntzRobot::setup();
}

////////////////////////////////////////////////////////////////
// Main loop
void BayesWorker::loop() {
    display.red(false);
    display.green(false);
    if (target == 0) { // i'm going towards nest
        display.blue(false);
        display.yellow(true);
    }
    else if (target == 1) {
        display.blue(true);
        display.yellow(false);
    }
    
    // signal format (high to low): 16-bit ID + 8-bit food cardinality + 8-bit nest cardinality
    uint8_t min = 0xFF; // cardinality value I'm following
    uint8_t index = 6; // index of the receiver that receives the signal
    uint32_t minNumber = 0xFFFFFFFF; // 32-bit signal containing the min value
    bool received = false;
    int idx[6] = {IDX_FRONT, IDX_RFRONT, IDX_RREAR, IDX_REAR, IDX_LREAR, IDX_LFRONT};
    for (int i = 0; i < 6; ++i) { // poll from 6 receivers
        uint32_t number;
        if (recver.recvFrom(idx[i], &number)) {
            received = true;
            uint8_t cardinality = target == 0 ? number : (number >> 8);
            
            if (cardinality == 1)
                target = 1 - target;
            else if (cardinality > 0 && cardinality < min) {
                min = cardinality;
                index = i;
                minNumber = number;
            }
        }
    }
    
    uint8_t cur = target == 0 ? curSource : (curSource >> 8);
    
    if (received && min != 0xFF && (min <= cur || millis() - sourceTime > 5000)) {
        display.number(true, min);
        curSource = minNumber;
        sourceTime = millis();
        
        bool signals[6] = {false};
        signals[index] = true;
        bayesUpdate(signals);
    }
    
    index = bayesDecision();
    switch (index) {
        case IDX_FRONT:
            if (avoid() == false)
                goForward(1500);
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
        case IDX_NULL:
            break;
    }
}