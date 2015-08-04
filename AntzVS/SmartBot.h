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
#define NEIGHBORS_COLLECTION_TIME_GUID 10 // number of iterations for collecting neighborhood info while being a guider
#define NEIGHBORS_COLLECTION_TIME_WORK 10  // number of iterations for collecting neighborhood info while being a worker

#define NO_SIGNAL	0xFF

namespace Antz
{
    class Role;

    class SmartBot : public AntzRobot
    {
        friend class Role;
        friend class WorkerRole;
        friend class GuiderRole;

    public:
        SmartBot(uint32_t robotId);
        ~SmartBot();

        void setup();
        void loop();
        void switchRole();

    private:
        void wipeNeighbors();
        void registerRobotSignal(Neighbor& robot, int sensor);
        void formNeighborhood();
        int countNeighbors();
        //bool isNeighborInArray(Neighbor& neighbor); // not used anymore
        bool isNeighborValid(Neighbor& neighbor);

        Role* robotRole;
        Neighbor* neighbors[6] = { NULL };
        Dll* seenRobots;
        int wipingNeighborsTimer; // timer for wiping neighbors array
        
        Sender sender;
        uint16_t minFood;
        uint16_t minNest;
        uint16_t curFood;
        uint16_t curNest;
    };
}

#endif /* defined(__Antz__SmartBot__) */
