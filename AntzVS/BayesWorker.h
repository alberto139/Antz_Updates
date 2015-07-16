//
//  BayesWorker.h
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.

// Doen's Work. Not finished?
//

#ifndef __Antz__BayesWorker__
#define __Antz__BayesWorker__

#include "AntzRobot.h"

namespace Antz {
    class BayesWorker: public AntzRobot {
    public:
        BayesWorker(uint32_t robotId);
        virtual ~BayesWorker() {}
        virtual void setup();
        virtual void loop();
    private:
        uint8_t target;
        uint32_t curSource;
        uint64_t sourceTime;
    };
}

#endif /* defined(__Antz__BayesWorker__) */
