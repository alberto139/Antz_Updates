/*
 * Antz
 * Antz Framework
 * Guider.cpp
 *		Definitions of various routines for Guider robot.
 *
 * Copyright: 2015 Ac Zhi Xing. All rights reserved.
 * License: <#license#>
 */

#include "Guider.h"
#include "Neighbor.h"

using namespace Antz;

/* Guider -- Constructor */
Guider::Guider(uint32_t robotId):
  AntzRobot(robotId),
  curFood(0xFF),
  curNest(0xFF),
  foodTimer(0),
  nestTimer(0),
  Wcount(0), // added 
  priority(DEFAULT_PRIORITY) {
}

/* setup -- setup routine for Guider robot */
void Guider::setup() {
  
        
    AntzRobot::setup();
    Serial.begin(9600);
    
    for (int i =0; i<6; i++) {            //Populate the array
          Neighborhood[i] = NULL;
        }
}

/* loop -- loop routine for Guider robot */
void Guider::loop() {
  
  //Serial.println("Guider::loop() ");
  //display.number(true, 8);
    stopMoving();
    minFood = 0xFF;			// to store the minimum food cardinality
    minNest = 0xFF;			// to store the minimum nest cardinality
    
    display.blue(false);
    display.yellow(false);
    display.red(true);		// when red LED is on, the robot listens to the signals from surroundings
    display.green(false);
    bool wait = true;		// a flag indicating whether there're more signals to be heard
	/* keep looping until message is heard */
    
    //*******commented out to just test neighbor recognition***********
          while (wait || minNest == 0xFF && minFood == 0xFF)
              wait = receiveSignal();
    
    //************************************************************
    
    //receiveSignal();
   //******************* Above is added code **********************
    
    delay(random(priority) * 10);
    
    
    //*******commented out to just test neighbor recognition***********
    if (!recver.canHearSignal()) {
        priority = DEFAULT_PRIORITY;
        display.red(false);
        display.green(true);		// when red LED turns off and green turns on, the robot starts sending the signal
	/* 
	 * We see two numbers flashing on a guider's display. One flashes for a 
	 * relatively longer duration while the other flashes for a fraction of a 
	 * moment. Number flashing for the longer duration is the number of guider 
	 * from food whereas the number which appears for a very short time is the 
	 * number from nest
	 */
        // display.number(true, curFood); !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // delay(100);
        // display.number(true, curNest);
	/* Display self position and forward the message to other guiders and workers */
        sendSignal();
    }
    else if (priority >= 5)
        priority -= 5;
}
    //************************************************************
    
    //sendSignal();
    //Serial.print("SENT SIGNALS: ");
    //}

    //******************* Above is added code **********************



/* receiveSignal -- receive signals from all the receivers */
bool Guider::receiveSignal()
{
    //Serial.println("Start Recieve");
    bool received = false;
    unsigned long cur = millis();	//millis() returns the number of milliseconds since the arduino began to run this program
    if (cur - nestTimer > 10000)
        curNest = 0xFF;
    if (cur - foodTimer > 10000)
        curFood = 0xFF;
        
    
    for (int i = 0; i < 6; ++i) 
    {	// 6 is the number of receiver sensors mounted on the robot
        if (recver.canHearSignal(i)) 
        {
            received = true;
            uint32_t number; // to store the 32-bit signal 
            if (recver.recvFrom(i, &number))
            {
                uint8_t nest = (uint8_t)(number & 0xFF);
                uint8_t food = (uint8_t)(number >> 8);
                if (nest > 0 && nest < minNest)
                    minNest = nest;
                if (food > 0 && food < minFood)
                    minFood = food;






                Neighbor* currentN = new Neighbor(number);
                if (isNeighborValid(*currentN))
                {
                    //Serial.println("Start NEW code");
                    //check to see if neighbor is in linkedlist already

                    /*
                    Serial.print("ID:");
                    Serial.println(currentN->id);
                    Serial.print("SEnsor:");
                    Serial.println(i);
                    Serial.print("Whole number:");
                    Serial.println(currentN->orgSignal, BIN);
                    */

                    DllIter* iter = list->createIterator();
                    bool added = false;
                    Neighbor* tempN;
                    while (!added && iter->hasNext())
                    {
                        tempN = iter->getNext();
                        if (tempN->id == currentN->id)
                        {
                            //Serial.println("Add to Linked List");
                            added = true;
                        }

                    }
                    if (added)
                    {
                        //Serial.println("Add to Linked List!!!!!!");
                        tempN->receivedFrom[i]++;
                        delete currentN;
                    }
                    else
                    {
                        currentN->receivedFrom[i]++;
                        list->pushFront(*currentN);
                    }
                    delete iter;
                }
                else
                    delete currentN;
            }
            

        } // end of if (recver.canHearSignal(i))

                
    } // end of for (int i = 0; i < 6; ++i) 

    

    // Wiping the neighborhood and populating with standard Neighbors with id -1
    if(Wcount > 100)
    {      // 10 might need to be changed


     while(!list->isEmpty()) //while(!list->IsEmpty() && countNeighbors() < 6)
        {
          DllIter* iter = list->createIterator();
          Neighbor* curMax = NULL;
          int curMaxIndex = 0;
          int maxVal = 0;
          
          while(iter->hasNext())
          {
            Neighbor* next = iter->getNext();
            int tempIndex = 0;
            
            
            for(int j = 0; j < 5; j++)
            {
             if(next->receivedFrom[tempIndex] < next->receivedFrom[j+1])
                tempIndex = j+1;
            }
            
            if(next->receivedFrom[tempIndex] > maxVal)
            {
              maxVal = next->receivedFrom[tempIndex];
              curMax = next;
              curMaxIndex = tempIndex;
            }
                
          }
          
          
          
        
          if(curMax != NULL && curMax->receivedFrom[curMaxIndex] > 0)
            {
              if(Neighborhood[curMaxIndex] == NULL)
              {
                Neighborhood[curMaxIndex] = curMax;
                list->remove(*curMax);
                //Serial.println(list->remove(*curMax));
              }else
              {
                curMax->receivedFrom[curMaxIndex] = 0;
                int sum = 0;
                for(int i=0;i<6;i++)
                  sum += curMax->receivedFrom[i];
                if(sum == 0)
                  list->remove(*curMax);
                  //Serial.println(list->remove(*curMax));
              }
            }
          delete iter;
        }





        DllIter* iter = list->createIterator();
        while(iter->hasNext())
        {
            Neighbor* temp = iter->getNext();
            /*
            Serial.print("element ");
            Serial.println(temp->id);
            Serial.println(" ---Sensor Count ----");
            for(int i = 0; i<6;i++)
                Serial.print(temp->receivedFrom[i]);
                */
            temp = iter->getNext();
        }
        delete iter;
        //Serial.println("Collecting information...");
        // All the information is recieved and stored
        // Now sort and add to Neighborhood  
//        for(int i = 0; i < 6; i++)
//        {
//            DllIter* iter = list->createIterator();
//            Neighbor* curMax = iter->getNext();
//            while(iter->hasNext())
//            {
//                Neighbor* next = iter->getNext();
//                if(curMax->receivedFrom[i] < next->receivedFrom[i])
//                    curMax = next;
//                    
//                    /*
//                Serial.print("\niter -> ID: ");
//                Serial.println(next->id);
//                Serial.print("iter -> val: ");
//                Serial.println(next->receivedFrom[i]);
//                Serial.print("Sensor #: ");
//                Serial.print("loop");
//                 */
//            }
//            /*
//            Serial.print("curMax Id: ");
//            Serial.println(curMax->id);
//            Serial.print("Sensor #: ");
//            Serial.println(i);
//            */
//            if(curMax != NULL && curMax->receivedFrom[i] > 0)
//            {
//                Neighborhood[i] = curMax;
//                //Serial.print("Is neighbor removed correctly? : ");
//                //Serial.println(list->remove(*curMax));
//                list->remove(*curMax);
//            }
//            delete iter;
//        }
    
        if(countNeighbors() >= 3)
        {
            if(!recalculation)
            {
                recalculation = true;
                //Serial.println("Recalculating...");
                delay(random(1000));
                Wcount = -15;
            }
            else
            {
                //Serial.println("I am going to Transition role");
                //transition();
            }
        }
        else
        {
            if(recalculation)
                recalculation = false;
            Wcount = 0;
        }
        
        for (int j = 0; j<6; j++)
            Neighborhood[j] = NULL;
        //Serial.println("-----------------------------------------");
        delete list;
        list = new Dll();
    }
    
    Wcount++;
        

    if (minNest < (uint16_t)0xFF && minNest + 1 <= curNest) {
        curNest = minNest + 1;
        nestTimer = millis();
    }
    if (minFood < (uint16_t)0xFF && minFood + 1 <= curFood) {
        curFood = minFood + 1; 
        foodTimer = millis();
    }
    return received;
}

/* sendSignal -- Send signals from all the senders */
void Guider::sendSignal() {
    uint32_t myNumber = 0;

    //Serial.println(myNumber, BIN);		
    myNumber |= (identifier << 16);
    myNumber |= ( curFood << 8);		
    myNumber |= curNest;

	/* message can be visualized as follows:
	 *		|----------------|--------|--------|
	 *		       16bits       8bits    8bits
	 *		  identifier       curFood  curNest
	 */
    sender.send(myNumber, 500);
    /*
    Serial.print("myNumber: ");
    Serial.println(myNumber, BIN);
    Serial.print("ID      : ");
    Serial.println(identifier, BIN);
    Serial.print("curFood : ");
    Serial.println(curFood, BIN);
    Serial.print("curNest : ");
    Serial.println(curNest, BIN);
    */

}

void Guider::transition(){
  AntzRobot *antz = AntzRobot::createAntzRobot("Worker", identifier);
  antz->setup();
  
  while(true){
    antz->loop();
  }
}

bool Guider::isNeighborValid(Neighbor& neighbor)
{
    return (neighbor.curFood == 0xFF || abs(neighbor.curFood - curFood) <= 2)
        && (neighbor.curNest == 0xFF || abs(neighbor.curNest - curNest) <= 2);
}
