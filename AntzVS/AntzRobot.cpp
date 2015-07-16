/*
 * Antz
 * Antz Framework
 * AntzRobot.cpp
 *		Definition of object factory and various other routines related to the robot.
 *		Contains routines mostly concerned with robot movement.
 *
 * Copyright (c) 2015 Zhi Xing. All rights reserved.
 * License: <#license#>
 */

#include "AntzRobot.h"
#include "Worker.h"
#include "Guider.h"
#include "ExpGuider.h"
#include "Tester.h"
#include "BayesWorker.h"

using namespace Antz;

uint8_t AntzRobot::avoidCnt = 0;
uint32_t AntzRobot::identifier = 0;
float AntzRobot::condProb[] = {1.f/6};
int64_t AntzRobot::motorStartMillis = -1;
int64_t AntzRobot::motorStopMillis = -1;
AntzRobot::MoveType AntzRobot::curMovement = mt_stop;
Display AntzRobot::display;
Motor AntzRobot::motor;
Receiver AntzRobot::recver;
Scanner AntzRobot::scanner;

/*
 * AntzRobot -- Constructor
 */
AntzRobot::AntzRobot(uint32_t robotId) {
    identifier = robotId;
}


/*
 * createAntzRobot -- Object factory
 * @ type:	specifies the type of robot required
 * @ robotId:	given Id of the robot to be created
 */
AntzRobot* AntzRobot::createAntzRobot(char* type, uint32_t robotId) {
    if (strcmp(type, "Worker") == 0)
        return new Worker(robotId);
    else if (strcmp(type, "Guider") == 0)
        return new Guider(robotId);
    else if (strcmp(type, "ExpGuider") == 0)
        return new ExpGuider(robotId);
    else if (strcmp(type, "Tester") == 0)
        return new Tester(robotId);
    else if (strcmp(type, "BayesWorker") == 0)
        return new BayesWorker(robotId);
    else
        return NULL;
}

/*
 * setup -- setup routine for Arduino sketch
 */
void AntzRobot::setup() {
    Timer3.initialize(100000); 		// run the ISR every 100 ms
    Timer3.attachInterrupt(isr);
    scanner.startup();
    randomSeed(analogRead(0));
}

/*
 * loop -- loop routine for Arduino sketch
 */
void AntzRobot::loop() {
}

/*
 * goForward -- move in forward direction for stipulated time
 * @ msec:	time in milli seconds
 * @ async:	flag to specify whether to move forward in synchronous or asynchronous manner
 */
void AntzRobot::goForward(uint64_t msecs, bool async) {
    bayesUpdate();
    Timer3.detachInterrupt();		// disable the timer interrupt
    motor.forward();			// perform the action
    curMovement = mt_forward;
    motorStartMillis = millis();
    motorStopMillis = motorStartMillis + msecs;
    Timer3.attachInterrupt(isr);	// enable the timer interrupt
    if (!async) {
        delay(msecs);			// introduce the delay if the action is synchronous
        stopMoving();
    }
}

/*
 * goBackward -- move in backward direction for stipulated time
 * @ msec:	time in milli seconds
 * @ async:	flag to specify whether to move backward in synchronous or asynchronous manner
 */
void AntzRobot::goBackward(uint64_t msecs, bool async) {
    bayesUpdate();
    Timer3.detachInterrupt();		// disable the timer interrupt
    motor.backward();			// perform the action
    curMovement = mt_backward;
    motorStartMillis = millis();
    motorStopMillis = motorStartMillis + msecs;
    Timer3.attachInterrupt(isr);	// enable the timer interrupt
    if (!async) {
        delay(msecs);			// introduce the delay if the action is synchronous
        stopMoving();
    }
}

/*
 * turnLeft --  turn left by stipulated degree
 * @ degree:	stipulated angle
 * @ async:	flag to specify whether to turn left in synchronous or asynchronous manner
 */
void AntzRobot::turnLeft(float degree, bool async) {
    bayesUpdate();
    Timer3.detachInterrupt();		// disable the timer interrupt
    motor.left();			// perform the action
    curMovement = mt_left;
    motorStartMillis = millis();
    motorStopMillis = motorStartMillis + MTR_MSPERDEG * degree;
    Timer3.attachInterrupt(isr);	// enable the timer interrupt
    if (!async) {
        delay(MTR_MSPERDEG * degree);	// introduce the delay if the action is synchronous
        stopMoving();
    }
}

/*
 * turnRight --  turn right by stipulated degree
 * @ degree:	stipulated angle
 * @ async:	flag to specify whether to turn right in synchronous or asynchronous manner
 */
void AntzRobot::turnRight(float degree, bool async) {
    bayesUpdate();
    Timer3.detachInterrupt();		// disable the timer interrupt
    motor.right();			// perform the action
    curMovement = mt_right;
    motorStartMillis = millis();
    motorStopMillis = motorStartMillis + MTR_MSPERDEG * degree;
    Timer3.attachInterrupt(isr);	// enable the timer interrupt
    if (!async) {
        delay(MTR_MSPERDEG * degree);	// introduce the delay of the action is synchronous
        stopMoving();
    }
}

/*
 * stopMoving -- immediately stop moving
 */
void AntzRobot::stopMoving() {
    bayesUpdate();
    Timer3.detachInterrupt();		// disable the timer interrupt
    motor.stop();			// perform the action
    curMovement = mt_stop;
    motorStartMillis = -1;
    motorStopMillis = -1;
    Timer3.attachInterrupt(isr);	// enable the timer interrupt
}

/*
 * avoid -- obstacle avoidance
 */
bool AntzRobot::avoid() {
    bool detected = false;
    float angle;
    bool async = true;
    uint8_t deg = 60;
    while (scanner.scan(&angle) <= 30) {// scanner.scan returns distance from obstacle and 30 is the tentative distance
        detected = true;
        ++avoidCnt;
	/*
	 * without this if condition, the robot's sensor can eternally keep shaking left-right left-right
	 * to break this deadlock, after shaking five times, the robot makes a movement
	 */
        if (avoidCnt > 5) { 		// possible deadlock
            async = false;
            deg = 90;
        }
        //if (angle > 30)
            turnRight(deg, async);
        //else
            //turnLeft(deg, async);
    }
    goForward(500);
    if (!detected)
        avoidCnt = 0;
    return detected;
}


/*
 * bayesUpdate -- Update conditional probabilities using Bayes rule according
 *   		  to new signals
 * @signals:	array of signals
 */
void AntzRobot::bayesUpdate(bool signals[]) {
    Timer3.detachInterrupt();
    for (int i = 0; i < 6; ++i) {
        if (signals[i]) {
            float marginal = PROB_TPOS * condProb[i] + PROB_FPOS * (1 - condProb[i]);
            condProb[i] = PROB_TPOS * condProb[i] / marginal;
        }
        else {
            float marginal = PROB_TNEG * (1 - condProb[i]) + PROB_FNEG * condProb[i];
            condProb[i] = 1 - PROB_TNEG * (1 - condProb[i]) / marginal;
        }
    }
    Timer3.attachInterrupt(isr);
}

/*
 * bayesUpdate -- update conditional probabilities using Bayes rule according
 *		  to new movement
 */
void AntzRobot::bayesUpdate() {
    int64_t duration = 0;
    Timer3.detachInterrupt();
    if (motorStartMillis > 0) {
        duration = millis() - motorStartMillis;
        motorStartMillis = millis();
    }

    if (curMovement == mt_forward || curMovement == mt_backward) {
        float avg = (1 - condProb[IDX_FRONT] - condProb[IDX_REAR]) / 4;
        condProb[IDX_LFRONT] = avg;
        condProb[IDX_LREAR] = avg;
        condProb[IDX_RFRONT] = avg;
        condProb[IDX_RREAR] = avg;
    }
    else if (curMovement == mt_left || curMovement == mt_right) {
        int shifts = (duration / MTR_MSPERDEG - 30) / 60 + 1;
        shifts = shifts % 6;
        float temp[shifts];
        if (curMovement == mt_left) { // array shifts right
            for (int i = 5; i >= 0; --i) {
                if (i + shifts >= 6)
                    temp[i + shifts - 6] = condProb[i];
                if (i - shifts >= 0)
                    condProb[i] = condProb[i - shifts];
                else
                    condProb[i] = temp[i];
            }
        }
        else if (curMovement == mt_right) { // array shifts left
            for (int i = 0; i < 6; ++i) {
                if (i - shifts < 0)
                    temp[i] = condProb[i];
                if (i + shifts < 6)
                    condProb[i] = condProb[i + shifts];
                else
                    condProb[i] = temp[i + shifts - 6];
            }
        }
    }
    Timer3.attachInterrupt(isr);
}

/*
 * bayesReset -- reset conditional probabilities
 *
 */
void AntzRobot::bayesReset() {
    Timer3.detachInterrupt();
    for (int i = 0; i < 6; ++i)
        condProb[i] = 1.f / 6;
    curMovement = mt_unknown;
    Timer3.attachInterrupt(isr);
}

/*
 * bayesDecision -- get the decision from the Bayesian model
 */
uint8_t AntzRobot::bayesDecision() {
    bayesUpdate();
    Timer3.detachInterrupt();
    int idx[] = {IDX_FRONT, IDX_LFRONT, IDX_RFRONT, IDX_LREAR, IDX_RREAR, IDX_REAR};
    for (int i = 0; i < 6; ++i) {
        if (condProb[idx[i]] > 0.8) {
            Timer3.attachInterrupt(isr);
            return idx[i];
        }
    }
    Timer3.attachInterrupt(isr);
    return IDX_NULL;
}

/*
 * isr -- the ISR for timer interrupts, this should be called in
 * 	  all the subclasses' ISRs
 */
void AntzRobot::isr() {
    if (motorStopMillis >= 0 && millis() >= motorStopMillis)
        stopMoving();
}
