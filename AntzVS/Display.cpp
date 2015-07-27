/*
 * Antz
 * Antz Framework
 * Display.cpp
 *	Definitions of various functions of display
 * 
 * Copyright: 2015 Ac Zhi Xing <zxing01@syr.edu>. All rights reserved.
 * License: <#license#>
 */

#include "Display.h"

using namespace Antz;

/* 
 * Display -- constructor 
 *	Initialize all the required pins as output pins
 */
Display::Display():curNum(0) {
    pinMode(HIGH1, OUTPUT);
    pinMode(HIGH2, OUTPUT);
    pinMode(TOP, OUTPUT);
    pinMode(MID, OUTPUT);
    pinMode(BOT, OUTPUT);
    pinMode(TOPLEFT, OUTPUT);
    pinMode(TOPRGHT, OUTPUT);
    pinMode(BOTLEFT, OUTPUT);
    pinMode(BOTRGHT, OUTPUT);
    pinMode(DOT, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(YELLOW, OUTPUT);
}

/* 
 * red -- function dealing with red led
 * @on:		flag telling what to do with the red led
 */
void Display::red(bool on) {
    if (on)
        digitalWrite(RED, HIGH);
    else
        digitalWrite(RED, LOW);
}

/* 
 * green -- function dealing with green led
 * @on:		flag telling what to do with the green led
 */
void Display::green(bool on) {
    if (on)
        digitalWrite(GREEN, HIGH);
    else
        digitalWrite(GREEN, LOW);
}

/* 
 * blue -- function dealing with blue led
 * @on:		flag telling what to do with the blue led
 */
void Display::blue(bool on) {
    if (on)
        digitalWrite(BLUE, HIGH);
    else
        digitalWrite(BLUE, LOW);
}

/* 
 * yellow -- function dealing with yellow led
 * @on:		flag telling what to do with the yellow led
 */
void Display::yellow(bool on) {
    if (on)
        digitalWrite(YELLOW, HIGH);
    else
        digitalWrite(YELLOW, LOW);
}

/*
 * number -- displays the specified number depending on the 'on' flag
 *	     it is a seven segment display display with a dot on bottom right
 * 
 *           --
 *	    |  |
 *           __
 *          |  |
 *           __  *
 *
 *	     every segment is an individual led and all of them are connected to a 5V power supply
 *	     so when an led is set to LOW, the potential difference gets created, the current flows and it glows
 * @on:		flag telling what to do with the led s constituting the curNum
 * @n:		the number to be displayed
 */
void Display::number(bool on, uint8_t n) {
    curNum = n;
    if (!on) {
        digitalWrite(HIGH1, LOW);
        digitalWrite(HIGH2, LOW);
    }
    else {
        digitalWrite(HIGH1, HIGH);
        digitalWrite(HIGH2, HIGH);
        switch (n) {
            case 0:				// display 0
                digitalWrite(TOP, LOW);		//
                digitalWrite(MID, HIGH);	//  --
                digitalWrite(BOT, LOW);		// |  |
                digitalWrite(TOPLEFT, LOW);	// |  | 
                digitalWrite(TOPRGHT, LOW);	//  --
                digitalWrite(BOTLEFT, LOW);	//
                digitalWrite(BOTRGHT, LOW);	//
                digitalWrite(DOT, HIGH);	//
                break;
            case 1:				// dispaly 1
                digitalWrite(TOP, HIGH);	//
                digitalWrite(MID, HIGH);	//    |
                digitalWrite(BOT, HIGH);	//    |
                digitalWrite(TOPLEFT, HIGH);	//
                digitalWrite(TOPRGHT, LOW);	//
                digitalWrite(BOTLEFT, HIGH);	//
                digitalWrite(BOTRGHT, LOW);	//
                digitalWrite(DOT, HIGH);	//
                break;
            case 2:				// display 2
                digitalWrite(TOP, LOW);		//
                digitalWrite(MID, LOW);		//  --
                digitalWrite(BOT, LOW);		//    |
                digitalWrite(TOPLEFT, HIGH);	//  --
                digitalWrite(TOPRGHT, LOW);	// | 
                digitalWrite(BOTLEFT, LOW);	//  __
                digitalWrite(BOTRGHT, HIGH);	//
                digitalWrite(DOT, HIGH);	//
                break;
            case 3:				// display 3
                digitalWrite(TOP, LOW);		//
                digitalWrite(MID, LOW);		//  __
                digitalWrite(BOT, LOW);		//    |
                digitalWrite(TOPLEFT, HIGH);	//  --
                digitalWrite(TOPRGHT, LOW);	//    |
                digitalWrite(BOTLEFT, HIGH);	//  --
                digitalWrite(BOTRGHT, LOW);	//
                digitalWrite(DOT, HIGH);	//
                break;
            case 4:				// display 4
                digitalWrite(TOP, HIGH);	// 
                digitalWrite(MID, LOW);		//  |  |
                digitalWrite(BOT, HIGH);	//   __
                digitalWrite(TOPLEFT, LOW);	//     |
                digitalWrite(TOPRGHT, LOW);	//
                digitalWrite(BOTLEFT, HIGH);	//	
                digitalWrite(BOTRGHT, LOW);	//
                digitalWrite(DOT, HIGH);	//
                break;
            case 5:				// display 5
                digitalWrite(TOP, LOW);		//
                digitalWrite(MID, LOW);		//  __
                digitalWrite(BOT, LOW);		// |
                digitalWrite(TOPLEFT, LOW);	//  __
                digitalWrite(TOPRGHT, HIGH);	//    |
                digitalWrite(BOTLEFT, HIGH);	//  --
                digitalWrite(BOTRGHT, LOW);	//
                digitalWrite(DOT, HIGH);	//
                break;	
            case 6:				// display 6
                digitalWrite(TOP, LOW);		//
                digitalWrite(MID, LOW);		//  __
                digitalWrite(BOT, LOW);		// |
                digitalWrite(TOPLEFT, LOW);	//  __
                digitalWrite(TOPRGHT, HIGH);	// |  |
                digitalWrite(BOTLEFT, LOW);	//  __
                digitalWrite(BOTRGHT, LOW);	//
                digitalWrite(DOT, HIGH);	//
                break;
            case 7:				// display 7
                digitalWrite(TOP, LOW);		//
                digitalWrite(MID, HIGH);	//  --
                digitalWrite(BOT, HIGH);	//    |
                digitalWrite(TOPLEFT, HIGH);	//    |
                digitalWrite(TOPRGHT, LOW);	//
                digitalWrite(BOTLEFT, HIGH);	//
                digitalWrite(BOTRGHT, LOW);	//
                digitalWrite(DOT, HIGH);	//
                break;	
            case 8:				// display 8
                digitalWrite(TOP, LOW);		//
                digitalWrite(MID, LOW);		//  --
                digitalWrite(BOT, LOW);		// |  |
                digitalWrite(TOPLEFT, LOW);	//  __
                digitalWrite(TOPRGHT, LOW);	// |  |
                digitalWrite(BOTLEFT, LOW);	//  __
                digitalWrite(BOTRGHT, LOW);	//
                digitalWrite(DOT, HIGH);	//
                break;	
            case 9:				// display 9
                digitalWrite(TOP, LOW);		//
                digitalWrite(MID, LOW);		//  __
                digitalWrite(BOT, LOW);		// |  |
                digitalWrite(TOPLEFT, LOW);	//  __
                digitalWrite(TOPRGHT, LOW);	//    |
                digitalWrite(BOTLEFT, HIGH);	//  __
                digitalWrite(BOTRGHT, LOW);	//
                digitalWrite(DOT, HIGH);	//
                break;	

              
            default:				// when the number is something other than above numbers
                digitalWrite(TOP, HIGH);	//
                digitalWrite(MID, HIGH);	//  
                digitalWrite(BOT, HIGH);	//  
                digitalWrite(TOPLEFT, HIGH);	// 
                digitalWrite(TOPRGHT, HIGH);	//      *
                digitalWrite(BOTLEFT, HIGH);	//
                digitalWrite(BOTRGHT, HIGH);	//
                digitalWrite(DOT, LOW);		//
                break;
        }
    }
}

/*
 * number -- displays the curNum number
 * @on:		flag telling what to do with the led s constituting the curNum
 */
void Display::number(bool on) {
    number(on, curNum);
}

/* currentNumber -- returns curNum */
uint8_t Display::currentNumber() {
    return curNum;
}

void Display::listeningForSignals()
{
    blue(false);
    yellow(false);
    red(true);
    green(false);
}

void Display::sendingSignal()
{
    red(false);
    green(true);
}

void Display::goingTowardsNest()
{
    red(false);
    green(false);
    blue(false);
    yellow(true);
}

void Display::goingTowardsFood()
{
    red(false);
    green(false);
    blue(true);
    yellow(false);
}
