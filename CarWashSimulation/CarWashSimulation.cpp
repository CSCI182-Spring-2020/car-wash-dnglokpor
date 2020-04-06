/*
CSCI 182 - Car Wash 1
CarWashSimulation.cpp
program that run simulations of a car wash using user inputed operation duration and car
income probability

Delwys Glokpor
v2.0 4/6/2020
*/

#include <iostream>
#include <time.h>
#include "SimpleList.h"
#include "IntTime.h"
#define WASH_TIME 3 // time needed to wash a car
using namespace std;

// signatures
bool newCarIn(int prob);
bool isOpen4Business(IntTime elapsedTime, IntTime operationTime);

int main()
{
    // user input
    int inputCarProb; 
    // working variables
    IntTime elapsedTime, operationDuration, washTime;
    bool washRoomIsFree;
    IntTime currentInTime, currentWaitTime; // current car infos
    SimpleList processingInTimes, processingWaitTimes; // waiting lists
    // after processing log and results
    // allow me to store intime, waitime and outtime for all cars washed
    SimpleList processedInTimes, processedWaitTimes, processedOutTimes;
    int washedCars, unwashedCars;
    IntTime avgWaitTime;

    // seed generator
    srand(time(NULL));

    // UI
    cout << "CAR WASH SIMULATION PROGRAM:" << endl;

    // get operation time and client probability
    cout << "For how long (in hours) should we simulate operation? ";
    cin >> operationDuration.hours;
    cout << "What is the probability of car coming in? ";
    cin >> inputCarProb;

    // start operation
    cout << "Simulation started for " << operationDuration.hours <<
        "h and a " << inputCarProb << "% chance of car coming in." << endl;

    // initialisation
    elapsedTime = IntTime(); // generates a 0d0h0m0s
    washRoomIsFree = true;

    while (isOpen4Business(elapsedTime, operationDuration)) {
        // clockwork
        elapsedTime.tick();

        // check for incoming car every minute
        if (elapsedTime.secs == 0) { // a minute has passed
            if (newCarIn(inputCarProb)) { // a new car came in
                // add car to waiting list
                // since the nodes can only store seconds then we convert time to seconds first
                processingInTimes.queueItem(elapsedTime.convertToSecs()); // record car in time
                processingWaitTimes.queueItem(0); // start counting its wait time
                elapsedTime.display();
                cout << "\t" << " A car came in" << endl;
            }
        }

        // wash car or wait for the washroom to be free
        if (washRoomIsFree) {
            if (processingWaitTimes.isEmpty()) { // no car waiting
                // simulation1.elapsedTime.display();
                // cout << "\t" << "No car to wash" << endl;
            }else { // cars waiting
                // put a car in the washing room and save its informations
                currentInTime = IntTime(IntTime::SEC, processingInTimes.deQueueItem());
                currentWaitTime = IntTime(IntTime::SEC, processingWaitTimes.deQueueItem());
                washTime.reset(); // reset the wash time
                washRoomIsFree = false; // mark the wash room as occupied
                elapsedTime.display();
                cout << "\t" << "Washing started." << endl;
            }
        }
        else { // washroom is occupied
            washTime.tick(); // keep washing
            if (washTime.mins < WASH_TIME) { // current car washing in progress
                // simulation1.elapsedTime.display();
                // cout << "\t" << "washing in progress" << endl;
            }
            else { // current car washing ended
                washRoomIsFree = true; // free room
                // log processed car infos
                processedInTimes.queueItem(currentInTime.convertToSecs());
                processedWaitTimes.queueItem(currentWaitTime.convertToSecs());
                processedOutTimes.queueItem(elapsedTime.convertToSecs()); // generate out time
                elapsedTime.display();
                cout << "\t" << "Washing ended." << endl;
            }
        }
        // make cars on the waiting list wait
        if (!processingWaitTimes.isEmpty())
            processingWaitTimes.wait();
    } // END OF SIMULATION

    // generate simulation statistics
    if (!washRoomIsFree) { // a car was being washed when simulation ended
        // move the car onto washed list
        processedInTimes.queueItem(currentInTime.convertToSecs());
        processedWaitTimes.queueItem(currentWaitTime.convertToSecs());
        processedOutTimes.queueItem(currentInTime.convertToSecs() + WASH_TIME);
    }
    washedCars = processedOutTimes.length();
    unwashedCars = processingWaitTimes.length();
    avgWaitTime.secs = processedWaitTimes.totalWaitTime(); // get total wait time in secs
    avgWaitTime.secs = avgWaitTime.secs / washedCars; // take integer average
    avgWaitTime.update(); // reformat

    // display results
    cout << endl << "SIMULATION RESULTS:" << endl;
    cout << "Duration: " << operationDuration.hours <<
        "\t|Probability of cars: " << inputCarProb << "%" << endl;
    cout << endl << "Washed " << washedCars << " cars." << endl;
    if (washedCars != 0) {
        cout << "Car no.|\t" << "Arrived|\t\t" << "Wait|\t" << "Left|" << endl;
        int i = 0;
        IntTime displayTime;
        while (!processedOutTimes.isEmpty()) {
            cout << ++i;
            displayTime = IntTime(IntTime::SEC, processedInTimes.deQueueItem());
            displayTime.update();
            cout << "\t\t"; displayTime.display(); // inlined two commands to make it easier to read
            displayTime = IntTime(IntTime::SEC, processedWaitTimes.deQueueItem());
            displayTime.update();
            cout << "\t\t"; displayTime.display();
            displayTime = IntTime(IntTime::SEC, processedOutTimes.deQueueItem());
            displayTime.update();
            cout << "\t\t"; displayTime.display();
            cout << endl;
        }
    }
    cout << endl << "Average wait time: "; avgWaitTime.display();
    cout << endl << endl << "Number of unwashed cars: " << unwashedCars << endl;
    if (unwashedCars != 0) {
        cout << "Car no.|\t" << "Arrived|\t\t" << "Wait|\t" << "Left|" << endl;
        
        int i = 0;
        IntTime displayTime;
        while (!processingWaitTimes.isEmpty()) {
            cout << ++i;
            displayTime = IntTime(IntTime::SEC, processingInTimes.deQueueItem());
            displayTime.update();
            cout << "\t\t"; displayTime.display(); // inlined two commands to make it easier to read
            displayTime = IntTime(IntTime::SEC, processingWaitTimes.deQueueItem());
            displayTime.update();
            cout << "\t\t"; displayTime.display();
            displayTime = IntTime(IntTime::SEC, 0); // waiting list doesn't have an out time
            displayTime.update();
            cout << "\t\t"; displayTime.display();
            cout << endl;
        }
    }
    cout << endl << endl << "END OF SIMULATION." << endl;
}

// external functions
bool newCarIn(int prob) {
    int currentProb = rand() % 100;

    return currentProb < prob;
}

bool isOpen4Business(IntTime elapsedTime, IntTime operationTime) {

    return elapsedTime.isLessThan(operationTime);
}