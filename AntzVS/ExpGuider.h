//
//  ExpGuider.h
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#ifndef __Antz__ExpGuider__
#define __Antz__ExpGuider__

#include "AntzRobot.h"

namespace Antz {
    class ExpGuider: public AntzRobot {
    public:
        ExpGuider(uint32_t robotId);
        virtual ~ExpGuider() {}
        virtual void setup();
        virtual void loop();
    private:
        virtual void signal();
        virtual void explore();
        Sender sender;
        uint8_t curFood;
        uint8_t curNest;
        uint64_t foodTimer;
        uint64_t nestTimer;
        uint8_t count;
        uint8_t priority;
    };
}

#endif /* defined(__Antz__ExpGuider__) */
