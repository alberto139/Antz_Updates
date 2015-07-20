/*
 * Antz
 * Antz Framework
 * Transition.cpp
 *		Definitions of various routines for Transition robot.
 *              This Transition happens in between role changes.
 *
 * Date: 7/17/15
 */

#include "Transition.h"

#include "Neighbor.h"

using namespace Antz;


/* Transition -- Constructor */
Transition::Transition(uint32_t robotId):
AntzRobot(robotId),
curFood(0xFF),
curNest(0xFF),
foodTimer(0),
nestTimer(0),
priority(DEFAULT_PRIORITY) {
}

/* setup -- setup routine for Transition robot */
void Transition::setup() {
    
    AntzRobot::setup();
    
}

/*loop -- loop routine for Transition robot */
void Transition::loop() {
    //stopMoving();
    Serial.println("I am in Transition role");
}

//void Transition::sendSignal() {}

//void Transition::receiveSignal(){}

