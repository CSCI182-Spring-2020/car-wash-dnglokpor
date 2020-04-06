#include "IntTime.h"
#include <iostream>
using namespace std;

// instantiate an IntTime with a preset value;
IntTime::IntTime(TimeComponent component, int value) {
    
    switch (component)
    {
        case SEC:
            secs = value;
            break;
        case MIN:
            mins = value;
            break;
        case HOUR:
            hours = value;
            break;
        default:
            days = value;
            break;
    }
    update();
}

// format "[DAYS]d:[HOURS]h:[MINS]m:[SECS]s"
void IntTime::update() {

    if (secs > 60 || secs % 60 == 0) {
        mins += secs / 60;
        secs %= 60;
    }
    if (mins > 60 || mins % 60 == 0) {
        hours += mins / 60;
        mins %= 60;
    }
    if (hours > 24 || hours % 24 == 0) {
        days += hours / 24;
        hours %= 24;
    }
}

// add a second to the time
void IntTime::tick() {

    secs++;
    update();
}

// reset time
void IntTime::reset() {

    days = hours = mins = secs = 0;
}

// convert time into seconds only for calculations
int IntTime::convertToSecs() {

    return days * 86400 + hours * 3600 + mins * 60 + secs;
}

// display the time in the console in format "[DAYS]d[HOURS]h[MINS]m[SECS]s"
void IntTime::display() {
    if (days > 0) {
        cout << days << "d";
    }
    if (hours > 0) {
        cout << hours << "h";
    }
    if (mins > 0) {
        cout << mins << "m";
    }
    cout << secs << "s";
}

// checks if current time is less than #otherTime
bool IntTime::isLessThan(IntTime otherTime) {
    return convertToSecs() < otherTime.convertToSecs();
}

IntTime& IntTime::operator+(const IntTime& oldTime)
{
    days += oldTime.days;
    hours += oldTime.hours;
    mins += oldTime.mins;
    secs += oldTime.secs;
    update();

    return *this;
}
