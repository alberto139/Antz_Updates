//
//  Sender.cpp
//  Antz
//
//  Created by Zhi Xing on 6/2/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#include "Sender.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
Sender::Sender() {
    TIMSK1 = 0; // disable timer 1 interrupt
    pinMode(TIMER_PWM_PIN, OUTPUT);
    digitalWrite(TIMER_PWM_PIN, HIGH); // When not sending PWM, we want it high (because the transistor will invert the output)
    randomSeed(analogRead(0));
}

////////////////////////////////////////////////////////////////
void Sender::send(uint32_t data, uint64_t duration) {
    uint8_t original = EIMSK;
    EIMSK = 0; // disable external interrupts
    
    TCCR1A = (1 << WGM11) | (1 << WGM10); // fast PWM, TOP = OCR1A
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10); // no prescaling
    OCR1A = SYSCLOCK / IR_KHZ;
    OCR1B = SYSCLOCK / IR_KHZ / 2;
    
    unsigned long cur = millis();
    do {
        signal(LEN_SIGN);
        for (int i = 0; i < NUM_BITS; i++) {
            interval(LEN_INTR);
            signal(((uint32_t)1 << i) & data ? LEN_ONE : LEN_ZERO);
        }
        interval(LEN_INTR * 3); // give a fixed interval first because random() has a big delay
        //interval(random(LEN_PRSV * 5)); // big interval between signals
    } while (millis() - cur < duration);
    
    EIMSK = original; // enable external interrupts
}

////////////////////////////////////////////////////////////////
void Sender::signal(uint32_t time) {
    TCCR1A |= (1 << COM1B1);
    delayMicroseconds(time);
}

////////////////////////////////////////////////////////////////
void Sender::interval(uint32_t time) {
    TCCR1A &= ~(1 << COM1B1);
    delayMicroseconds(time);
}
