/*
 * Antz
 * Antz Framework
 * Display.h
 *	Declaration of class Display in namespace Antz
 *
 * Copyright: 2015 Ac Zhi Xing <zxing01@syr.edu>. All rights reserved.
 * License: <#license#>
 */


#ifndef __Antz__Display__
#define __Antz__Display__

#include <Arduino.h>

/* Pin numbers of Arduino board where the respective display lights are connected */
#define HIGH1       24
#define HIGH2       29
#define TOP         25
#define MID         22
#define BOT         30
#define TOPLEFT     23
#define TOPRGHT     26
#define BOTLEFT     31
#define BOTRGHT     28
#define DOT         27
#define RED         35
#define GREEN       33
#define BLUE        34
#define YELLOW      32


namespace Antz {
    class Display {
    public:
        Display();
        void red(bool on);
        void green(bool on);
        void blue(bool on);
        void yellow(bool on);
        void number(bool on, uint8_t n); // display n if n is in [1, 9], display 0 otherwise
        void number(bool on);
        uint8_t currentNumber();

        void listeningForSignals();
        void sendingSignal();
        void goingTowardsNest();
        void goingTowardsFood();
    private:
        uint8_t curNum;
    };
}

#endif /* defined(__Antz__Display__) */
