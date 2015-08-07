/* 
 * Antz
 * Antz Framework
 * AntzVS.ino
 *
 * Copyright: 2015 � Zhi Xing <zxing01@syr.edu>
 * Licence		<#license#>
*/


/*
 * Getting Started:
 *	www.arduino.cc/en/Guide/HomePage
 *
 * Development Enviroment:
 *	www.arduino.cc/en/Guide/Environment
 *	www.arduino.cc/en/Tutorial/Sketch
 *	www.arduino.cc/en/Tutorial/BareMinimum
 *
 * Programming References:
 *	www.arduino.cc/en/Reference/HomePage
 *
 */

#include <Servo.h>

#include "Display.h"
#include "AntzRobot.h"

#define ID ((uint32_t)2) //change from 32 to 16

//Display AntzRobot::display;

using namespace Antz;

/* Create a Worker ant */
AntzRobot *antz = AntzRobot::createAntzRobot("Tester", ID);

/*
 * setup - constructor of the sketch
 * This function is called when the sketch starts. This function is
 * used to initialize the values. This is the constructor of the sketch.
 *	www.arduino.cc/en/Reference/setup
*/
void setup() {
	antz->setup();
        Serial.begin(9600);
        Serial.println("I'm Here. In setup() ");

}

/*
 *  loop - loops continuously
 *  As the name suggests, loop function keeps looping continuously and performs
 *  stipulated operation/s.
 *	www.arduino.cc/en/Reference/setup
 */
void loop() {
	antz->loop();
       //antz->goForward(1000,1);
       //antz->display(true, 8);
}
