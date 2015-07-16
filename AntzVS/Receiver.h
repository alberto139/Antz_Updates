//
//  Receiver.h
//  Antz
//
//  Created by Zhi Xing on 6/2/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Receiver__
#define __Antz__Receiver__

// state machine states
#define STATE_IDLE      0
#define STATE_SIGN      1
#define STATE_INTR      2
#define STATE_DATA      3
#define STATE_DONE      4
// interrupt pins
#define RECV_PIN0       21
#define RECV_PIN1       20
#define RECV_PIN2       19
#define RECV_PIN3       18
#define RECV_PIN4       2
#define RECV_PIN5       3
//#define RECV_PIN0       21
//#define RECV_PIN1       3
//#define RECV_PIN2       20
//#define RECV_PIN3       2
//#define RECV_PIN4       19
//#define RECV_PIN5       18

// signal tolerance
#define TOLERANCE       0.2
#define LOW_LEN(us)     (int) (us * (1.0 - TOLERANCE))
#define HIGH_LEN(us)    (int) (us * (1.0 + TOLERANCE))
// switch pin
#define POWER           52
#define RESET_THR       10

#include <avr/interrupt.h>
#include <Arduino.h>
#include "Communication.h"

namespace Antz {
    class Receiver {
    public:
        struct RecvState {
            RecvState(uint8_t intn, uint8_t iscn0, volatile uint8_t &eicrx)
            :INTn(intn), ISCn0(iscn0), EICRx(eicrx), counter(0) {}
            uint32_t data;   // output value, only valid when state is STATE_DONE
            uint8_t state;	// state of state machine
            uint8_t bit;    // the bit being received
            uint32_t start;	// start time of last signal
            uint8_t INTn;
            uint8_t ISCn0;
            volatile uint8_t &EICRx;
            uint8_t counter; // count of null signals: if there has been more than RESET_THR, reset the receivers
            //uint32_t duration[NUM_BITS];
        };
        
        Receiver();
        void turnOnAll(bool on);
        bool recvFrom(uint8_t index, uint32_t *value); // blocking call, receive data from receiver #index
        bool recvFromNonBlocking(uint8_t index, uint32_t *value); // non-blocking call, assuming turnOnAll(true) has been called sometime earlier
        bool canHearSignal(); // non-blocking call, quickly check if there's any signal on all the receivers
        bool canHearSignal(uint8_t);
        
        static void stateTransit(volatile RecvState &recver);
        static volatile RecvState recver0;
        static volatile RecvState recver1;
        static volatile RecvState recver2;
        static volatile RecvState recver3;
        static volatile RecvState recver4;
        static volatile RecvState recver5;
    private:
        bool getData(volatile RecvState &recver, uint32_t *value);
        bool getDataNonBlocking(volatile RecvState &recver, uint32_t *value);
    };
}

#endif
