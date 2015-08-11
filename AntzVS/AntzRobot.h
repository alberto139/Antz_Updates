/*
 * Antz
 * Antz Framework
 * AntzRobot.h
 *		Declaration of namespace Antz and class AntzRobot with some constant values
 *
 * Copyright: 2015 Ac Zhi Xing <zxing01@syr.edu>. All rights reserved.
 * License: <#license#>
 */


#ifndef __Antz__AntzRobot__
#define __Antz__AntzRobot__

#include "Display.h"
#include "Motor.h"
#include "Receiver.h"
#include "Scanner.h"
#include "Sender.h"
#include "TimerThree.h"
#include "dll.h"


#include "Neighbor.h" // added 

#define IDX_FRONT       0
#define IDX_RFRONT      1
#define IDX_RREAR       2
#define IDX_REAR        3
#define IDX_LREAR       4
#define IDX_LFRONT      5
#define IDX_NULL        6
#define MTR_MSPERDEG    9
#define PROB_TPOS       0.95f
#define PROB_FNEG       0.05f
#define PROB_TNEG       (2.f/3)
#define PROB_FPOS       (1.f/3)
#define SIG_MAXLEN      ((uint32_t)LEN_SIGN + NUM_BITS * (LEN_ONE + LEN_INTR))

namespace Antz {
    class AntzRobot {

    public:
      
	/* Types of movements a robot can perform */
        enum MoveType {mt_forward, mt_backward, mt_left, mt_right, mt_stop, mt_unknown};
        virtual ~AntzRobot() {}
        virtual void setup();
        virtual void loop();
        static AntzRobot* createAntzRobot(char* type, uint32_t robotId);
        
        Neighbor* Neighborhood [6];    //Array to keep track of Neighbors
        Dll *list;

    protected:
    //public:
        AntzRobot(uint32_t robotId);
        static void goForward(uint64_t msecs, bool async = true);
        static void goBackward(uint64_t msecs, bool async = true);
        static void turnLeft(float degree, bool async = true);
        static void turnRight(float degree, bool async = true);
        static void stopMoving();
        static bool avoid();

        bool blocked(); // Added by Eli
        void evasiveAction(); // Added by Eli
        bool obstacleSeen = false;
        /*
		 * The six signal should be: front, right front, right rear
         * rear, left rear, and left front
		 */
        
    
        static void bayesUpdate(bool signals[]);
        static void bayesUpdate();
        static void bayesReset();
        static uint8_t bayesDecision();
        static void isr();

        virtual int countNeighbors();
        virtual bool isNeighborInArray(Neighbor& neighbor);

        static uint32_t identifier;
        static int64_t motorStartMillis;
        static int64_t motorStopMillis;
        static uint8_t avoidCnt;
        static float condProb[6];
        static MoveType curMovement;
        static Display display;
        static Motor motor;
        static Receiver recver;
        static Scanner scanner;
		Sender sender;
    };
}
#endif /* defined(__Antz__AntzRobot__) */
