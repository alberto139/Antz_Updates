//
//  Scanner.h
//  Antz
//
//  Created by Zhi Xing on 7/1/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Scanner__
#define __Antz__Scanner__

#define IR_PIN A15
#define SERVO_PIN     53
#define SERVO_SPD     (120.f / 60.f) // per degree
#define MIN_POS       45.f  //15.f // !!!! FOR LINE ROLE ONLY !!!!!!
#define MAX_POS       135.f  //165.f // !!!! FOR LINE ROLE ONLY !!!!!!
#define NUM_OF_STOPS  3 // changed from 5
#define INTERVAL      ((MAX_POS - MIN_POS) / (NUM_OF_STOPS - 1))

#include <Arduino.h>
#include <Servo.h>

namespace Antz {
    class Infrared {
    public:
        Infrared(uint8_t nReadings = 5);
        float getDistance(); // in centimeter(s)
    private:
        uint8_t _nReadings;
    };
    
    class Scanner {
    public:
        Scanner();
        void startup();
        float scan(float* angle);
        Infrared ir; // changed from private for debugging
    private:
        bool forward;
        Servo servo;
        
    };
}

#endif
