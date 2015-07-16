//
//  Receiver.cpp
//  Antz
//
//  Created by Zhi Xing on 6/2/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#include "Receiver.h"
//#define DEBUG

using namespace Antz;

////////////////////////////////////////////////////////////////
volatile Receiver::RecvState Receiver::recver0(INT0, ISC00, EICRA);
volatile Receiver::RecvState Receiver::recver1(INT1, ISC10, EICRA);
volatile Receiver::RecvState Receiver::recver2(INT2, ISC20, EICRA);
volatile Receiver::RecvState Receiver::recver3(INT3, ISC30, EICRA);
volatile Receiver::RecvState Receiver::recver4(INT4, ISC40, EICRB);
volatile Receiver::RecvState Receiver::recver5(INT5, ISC50, EICRB);

//volatile Receiver::RecvState Receiver::recver0(INT0, ISC00, EICRA);
//volatile Receiver::RecvState Receiver::recver1(INT5, ISC50, EICRB);
//volatile Receiver::RecvState Receiver::recver2(INT1, ISC10, EICRA);
//volatile Receiver::RecvState Receiver::recver3(INT4, ISC40, EICRB);
//volatile Receiver::RecvState Receiver::recver4(INT2, ISC20, EICRA);
//volatile Receiver::RecvState Receiver::recver5(INT3, ISC30, EICRA);

////////////////////////////////////////////////////////////////
Receiver::Receiver() {
    pinMode(RECV_PIN0, INPUT);
    pinMode(RECV_PIN1, INPUT);
    pinMode(RECV_PIN2, INPUT);
    pinMode(RECV_PIN3, INPUT);
    pinMode(RECV_PIN4, INPUT);
    pinMode(RECV_PIN5, INPUT);
    pinMode(POWER, OUTPUT);
    digitalWrite(POWER, HIGH);
    
    EICRA = (1 << ISC01) + (1 << ISC11) + (1 << ISC21) + (1 << ISC31);
    EICRB = (1 << ISC41) + (1 << ISC51);
    
    // all interrupts are initially disabled
    //EIMSK = (1 << INT0) + (1 << INT1) + (1 << INT2) + (1 << INT3) + (1 << INT4) + (1 << INT5);
}

////////////////////////////////////////////////////////////////
void Receiver::turnOnAll(bool on) {
    if (on)
        EIMSK |= ((1 << INT0) + (1 << INT1) + (1 << INT2) + (1 << INT3) + (1 << INT4) + (1 << INT5));
    else
        EIMSK &= ~((1 << INT0) + (1 << INT1) + (1 << INT2) + (1 << INT3) + (1 << INT4) + (1 << INT5));
}

////////////////////////////////////////////////////////////////
bool Receiver::recvFrom(uint8_t index, uint32_t *value) {
    switch (index) {
        case 0:
            return getData(recver0, value);
        case 1:
            return getData(recver1, value);
        case 2:
            return getData(recver2, value);
        case 3:
            return getData(recver3, value);
        case 4:
            return getData(recver4, value);
        case 5:
            return getData(recver5, value);
    }
}

////////////////////////////////////////////////////////////////
bool Receiver::getData(volatile RecvState &recver, uint32_t *value) {
    EIMSK |= (1 << recver.INTn); // enable interrupt for the receiver
    unsigned long start = micros();
    do {
        if (recver.state == STATE_DONE) {
            EIMSK &= ~(1 << recver.INTn); // disable interrupt for the receiver
            if (value)
                *value = recver.data;
            recver.EICRx &= ~(1 << recver.ISCn0);
            recver.state = STATE_IDLE;
            recver.data = 0;
            recver.bit = 0;
            
            recver.counter = 0;
            return true;
        }
    } while (micros() - start < LEN_PRSV * 5);
    EIMSK &= ~(1 << recver.INTn); // disable interrupt for the receiver
    
    if (++recver.counter > RESET_THR) {
        digitalWrite(POWER, LOW);
        delay(5); // a delay for the receivers to cool off
        digitalWrite(POWER, HIGH);
        recver.counter = 0;
    }
    return false;
}

////////////////////////////////////////////////////////////////
bool Receiver::recvFromNonBlocking(uint8_t index, uint32_t *value) {
    switch (index) {
        case 0:
            return getDataNonBlocking(recver0, value);
        case 1:
            return getDataNonBlocking(recver1, value);
        case 2:
            return getDataNonBlocking(recver2, value);
        case 3:
            return getDataNonBlocking(recver3, value);
        case 4:
            return getDataNonBlocking(recver4, value);
        case 5:
            return getDataNonBlocking(recver5, value);
    }
}

////////////////////////////////////////////////////////////////
bool Receiver::getDataNonBlocking(volatile RecvState &recver, uint32_t *value) {
    if (recver.state == STATE_DONE) {
        EIMSK &= ~(1 << recver.INTn); // disable interrupt for the receiver
        if (value)
            *value = recver.data;
        recver.EICRx &= ~(1 << recver.ISCn0);
        recver.state = STATE_IDLE;
        recver.data = 0;
        recver.bit = 0;
        EIMSK |= (1 << recver.INTn); // enable interrupt for the receiver
        recver.counter = 0;
        return true;
    }
    
    if (++recver.counter > RESET_THR) {
        digitalWrite(POWER, LOW);
        delay(5); // a delay for the receivers to cool off
        digitalWrite(POWER, HIGH);
        recver.counter = 0;
    }
    return false;
}

////////////////////////////////////////////////////////////////
bool Receiver::canHearSignal() {    
    return !(digitalRead(RECV_PIN0) && digitalRead(RECV_PIN1) && digitalRead(RECV_PIN2)
             && digitalRead(RECV_PIN3) && digitalRead(RECV_PIN4) && digitalRead(RECV_PIN5));
}

////////////////////////////////////////////////////////////////
bool Receiver::canHearSignal(uint8_t index) {
    switch (index) {
        case 0:
            return !digitalRead(RECV_PIN0);
        case 1:
            return !digitalRead(RECV_PIN1);
        case 2:
            return !digitalRead(RECV_PIN2);
        case 3:
            return !digitalRead(RECV_PIN3);
        case 4:
            return !digitalRead(RECV_PIN4);
        case 5:
            return !digitalRead(RECV_PIN5);
    }
}

////////////////////////////////////////////////////////////////
void Receiver::stateTransit(volatile RecvState &recver) {
    uint32_t time = micros();
    uint32_t duration = time - recver.start; // duration of signal
    recver.start = time; // record starting time
    //recver.duration[recver.bit] = duration;
    
    switch (recver.state) {
        case STATE_IDLE:
            recver.EICRx |= (1 << recver.ISCn0); // change event to raising edge
            recver.state = STATE_SIGN; // receiving signature
            break;
            
        case STATE_SIGN:
            recver.EICRx &= ~(1 << recver.ISCn0); // change event to falling edge
            if (duration >= LOW_LEN(LEN_SIGN) && duration <= HIGH_LEN(LEN_SIGN)) {
                recver.state = STATE_INTR; // received signature, receiving interval
            }
            else {
                recver.state = STATE_IDLE; // go back to idle
                recver.data = 0;
                recver.bit = 0;
            }
            break;
            
        case STATE_INTR:
            recver.EICRx |= (1 << recver.ISCn0);
            if (duration >= LOW_LEN(LEN_INTR) && duration <= HIGH_LEN(LEN_INTR)) {
                recver.state = STATE_DATA; // received interval, receiving data
            }
            else {
                recver.state = STATE_IDLE;
                recver.data = 0;
                recver.bit = 0;
            }
            break;
            
        case STATE_DATA:
            recver.EICRx &= ~(1 << recver.ISCn0);
            if (duration >= LOW_LEN(LEN_ONE) && duration <= HIGH_LEN(LEN_ONE) && recver.bit < NUM_BITS) {
                recver.state = STATE_INTR;
                recver.data |= ((uint32_t)1 << recver.bit++);
            }
            else if (duration >= LOW_LEN(LEN_ZERO) && duration <= HIGH_LEN(LEN_ZERO) && recver.bit < NUM_BITS) {
                recver.state = STATE_INTR;
                recver.data &= ~((uint32_t)1 << recver.bit++);
            }
            else {
                recver.state = STATE_IDLE;
                recver.data = 0;
                recver.bit = 0;
            }
            if (recver.bit == NUM_BITS) {
                recver.state = STATE_DONE;
            }
            break;

        case STATE_DONE:
            if (duration >= LEN_PRSV) {
                recver.EICRx |= (1 << recver.ISCn0);
                recver.state = STATE_SIGN;
                recver.data = 0;
                recver.bit = 0;
            }
            break;
    }
}

////////////////////////////////////////////////////////////////
ISR(INT0_vect) {
    Receiver::stateTransit(Receiver::recver0);
}

////////////////////////////////////////////////////////////////
ISR(INT1_vect) {
    Receiver::stateTransit(Receiver::recver1);
}

////////////////////////////////////////////////////////////////
ISR(INT2_vect) {
    Receiver::stateTransit(Receiver::recver2);
}

////////////////////////////////////////////////////////////////
ISR(INT3_vect) {
    Receiver::stateTransit(Receiver::recver3);
}

////////////////////////////////////////////////////////////////
ISR(INT4_vect) {
    Receiver::stateTransit(Receiver::recver4);
}

////////////////////////////////////////////////////////////////
ISR(INT5_vect) {
    Receiver::stateTransit(Receiver::recver5);
}
