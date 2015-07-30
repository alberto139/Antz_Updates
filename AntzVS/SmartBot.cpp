//
//  SmartBot.cpp
//  Antz
//
//  Created by Eli Goldweber on 7/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "SmartBot.h"
#include "Neighbor.h"
#include "Role.h"
#include "WorkerRole.h"
#include "GuiderRole.h"

using namespace Antz;

/* SmartBot -- Constructor */
SmartBot::SmartBot(uint32_t robotId):
    AntzRobot(robotId),
    curFood(NO_SIGNAL),
    curNest(NO_SIGNAL)
{
    robotRole = new WorkerRole(*this);
    
    //wipeNeighbors();
      for (int i = 0; i < 6; i++)
    {
        //delete neighbors[i];
        neighbors[i] = NULL;
    }
}

SmartBot::~SmartBot()
{
    wipeNeighbors();
    delete robotRole;
}

/* setup -- setup routine for SmartBot robot */
void SmartBot::setup()
{
    AntzRobot::setup();
    Serial.begin(9600);
}

/* loop -- loop routine for SmartBot robot */
void SmartBot::loop()
{
    if (robotRole->makeStep() == SWITCH_ROLE)
        switchRole();
}

void SmartBot::switchRole()
{
    int roleId = robotRole->getRoleId();
    delete robotRole;
    if (roleId == ROLE_WORKER)
        robotRole = new GuiderRole(*this);
    else
        robotRole = new WorkerRole(*this);
}

void SmartBot::wipeNeighbors()
{
    for (int i = 0; i < 6; i++)
    {
        delete neighbors[i];
        neighbors[i] = NULL;
    }
}

int SmartBot::countNeighbors()
{
    int neighborCount = 0;

    //Printf of Neighborhood array
    Serial.println("Neighborhood array");
    for (int i = 0; i < 6; i++)
    {
        Serial.print(neighbors[i] != NULL ? '_' : (char)(neighbors[i]->id + '0'));
        Serial.print("  ");
        if (neighbors[i] != NULL)
            neighborCount++;
    }
    Serial.println("");
    Serial.print("# Neighbors: ");
    Serial.println(neighborCount);
    //End of Printf of Neighborhood array

    return neighborCount;
}
bool SmartBot::isNeighborInArray(Neighbor& neighbor)
{
    for (int i = 0; i < 6; i++)
        if (neighbors[i] != NULL && neighbors[i]->id == neighbor.id)
            return true;
    return false;
}
