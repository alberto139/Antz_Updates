//
//  SmartBot.h
//  Antz
//
//  Created by Eli Goldweber on 7/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#ifndef __Antz__SmartBot__
#define __Antz__SmartBot__

#include "AntzRobot.h"

#define DEFAULT_PRIORITY 50

namespace Antz {
    class SmartBot: public AntzRobot {
    public:
        SmartBot(uint32_t robotId);
        virtual ~SmartBot() {}
        virtual void setup();
        virtual void loop();
        
        //guider
        int Wcount; //counter for wiping Neighborhood
        //int neighborCount;
    protected:
        
        //Guider stuff
        
        //virtual bool transition();
        virtual bool receiveSignal();
        virtual void sendSignal();
        
        virtual void beaconLoop();
        
        Sender sender;
        uint16_t minFood;
        uint16_t minNest;
        uint16_t curFood;
        uint16_t curNest;
        uint64_t foodTimer;
        uint64_t nestTimer;
        uint8_t priority;
        bool beacon;
        
        
      //Worker stuff
        
        virtual void workerLoop();
        virtual bool receiveSignalWorker();
        virtual void makeMovement();
        virtual void randomWalkGo();
        virtual void randomWalkReset();
        
        uint64_t randomMoveTimer;
        uint8_t target;
        uint32_t curNumber;
        uint64_t numberTimer;
        uint8_t minSignal;
        uint8_t signalIndex;
        uint32_t minNumber;
        uint8_t foodIndex;
        uint8_t nestIndex;
        uint16_t movePhase;
        uint8_t noMoveCnt;
        
        //worker
    };
}

#endif /* defined(__Antz__SmartBot__) */
