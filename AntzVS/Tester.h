//
//  Tester.h
//  Antz
//
//  Created by Zhi Xing on 5/3/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Tester__
#define __Antz__Tester__

#include "AntzRobot.h"

namespace Antz {
    class Tester: public AntzRobot {
    public:
        Tester(uint32_t robotId);
        virtual ~Tester() {}
        virtual void setup();
        virtual void loop();
        virtual void bayesPrint(const char *description);
    };
}


#endif /* defined(__Antz__Tester__) */
