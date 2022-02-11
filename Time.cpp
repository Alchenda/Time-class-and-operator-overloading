//
//  Time.cpp
//  Program3
//
//  Created by Danny Webb-McClain on 10/13/21.
//

/*
 * EECE.3220: Data Structures
 * Instructor: M. Geiger
 *
 * Time.cpp: Time function definitions
 * Includes blank definitions for overloaded operators to be written for Program 3
 */

#include "Time.hpp"       // Necessary for Time class definition; implicitly includes <iostream>
#include <iomanip>    // Necessary for setw(), setfill()
#include <math.h>
using std::setw;
using std::setfill;

/*** OVERLOADED OPERATORS TO BE ADDED FOR PROGRAM 3 ***/
/*** PREVIOUSLY DEFINED FUNCTIONS START ON LINE 145 (BEFORE YOU START ADDING CODE) ***/
/*** UPDATED 10/11 TO FIX PARAMETERIZED CONSTRUCTOR, advance() ***/

// Output operator
ostream& operator <<(ostream& out, const Time& rhs) {
    out << rhs.hours << ":" << setw(2) << setfill('0') << rhs.minutes << " " << rhs.AMorPM << "M";
    /*************************************************
    * Print time using form:
    *    h:mm _M  or hh:mm _M
    * where:
    *    h or hh    = # of hours (1 or 2 digits)
    *    mm            = # of minutes (always 2 digits)
    *    _M            = AM or PM
    **************************************************/

    return out;
}

// Input operator
istream& operator >>(istream& in, Time& rhs) {
    in >> rhs.hours;
    in.ignore(1); // need to ignore the colon when loading in the values of time
    in >> rhs.minutes >> rhs.AMorPM;
    in.ignore(1);//need to ignore 'M' since only a single character to indicate time of day is stored
    if (rhs.AMorPM == 'P') {//automatically convert to mil time
        rhs.miltime = 100 * (rhs.hours + 12) + rhs.minutes;
    } else{
        rhs.miltime = 100 * rhs.hours + rhs.minutes;
    }
    
    /*************************************************
    * Read time assuming it is written in form:
    *    h:mm _M  or hh:mm _M
    * where:
    *    h or hh    = # of hours (1 or 2 digits)
    *    mm            = # of minutes (always 2 digits)
    *    _M            = AM or PM
    **************************************************/

    return in;
}

// Comparison operators
bool Time::operator ==(const Time& rhs) {
    if (hours == rhs.hours && minutes == rhs.minutes && AMorPM == rhs.AMorPM) {
        return true;
    } else{
        return false;
    }
    /********************************************
    * Returns true if calling object matches rhs,
    *   false otherwise
    *********************************************/

    
}

bool Time::operator !=(const Time& rhs) {

    /**************************************************
    * Returns true if calling object doesn't match rhs,
    *   false otherwise
    ***************************************************/

    return (!(*this == rhs));
}
//comparing A to P is fine. since A has an ascii value lower than P, times in the morning will automatically be less than times in the afternoon/evening
bool Time::operator <(const Time& rhs) {
    if (AMorPM < rhs.AMorPM) { // rhs is in afternoon, lhs is in morning
        return true;
    } else if(AMorPM > rhs.AMorPM){ // lhs is in afternoon, rhs is in morning
        return false;
    } else{ //this is state in which both rhs and lhs are in the same day period
        if (hours > rhs.hours) {
            return false;
        } else if ((hours == rhs.hours) && (minutes > rhs.minutes)){ // this is the state in which lhs hours is same as rhs hours but minutes is larger
            return false;
        } else if (hours < rhs.hours){ //rhs hours is greater so lhs is already less than rhs
            return true;
        } else{
            return false;
        }
    }
    /**********************************************
    * Returns true if calling object is less
    *   (earlier in day) than rhs, false otherwise
    ***********************************************/

    return false;
}

bool Time::operator >(const Time& rhs) {

    /********************************************
    * Returns true if calling object is greater
    *   (later in day) than rhs, false otherwise
    *********************************************/

    return (!(*this < rhs) && (*this != rhs));
}

// Arithmetic operators
Time Time::operator +(const Time& rhs) { //reference advance function for documentation, the functions are fundamentally the same
    Time sum;
    if (minutes + rhs.minutes >= 60)
        sum.miltime = (rhs.miltime + miltime + 40) % 2400;
    else
        sum.miltime = (rhs.miltime + miltime) % 2400;
    
    sum.hours = sum.miltime / 100;
    
    if (sum.hours > 12) {
        sum.hours = sum.hours - 12;
    } else if (sum.hours == 0)
        sum.hours = 12;
    
    sum.minutes = sum.miltime % 100;
    if ((sum.miltime >= 0000 && sum.miltime < 1200) || (sum.miltime >= 2400 && sum.miltime < 3600)) {
        sum.AMorPM = 'A';
    } else if((sum.miltime < 2400 && sum.miltime >= 1200) || ((sum.miltime >= 3600) && (sum.miltime < 4800))){
        sum.AMorPM = 'P';
    }
    
    
    /********************************************
    * Add two Time objects and return sum
    *   See examples in spec
    *********************************************/

    return sum;
}


Time Time::operator -(const Time& rhs) {
    Time diff;
    bool flipFlag = 0;
    bool escape = 0;
    bool pass = 0;
    if (rhs.minutes > minutes) {
        flipFlag = 1;
    }
    
    if (rhs.minutes > minutes) {
        if (rhs.miltime > miltime) {
            diff.miltime = (rhs.miltime - miltime - 40) % 2400;
        }else{
            diff.miltime = (miltime - rhs.miltime - 40) % 2400;
            escape = 1;
        }

    }else{
       diff.miltime = (rhs.miltime - miltime) % 2400;
        //diff.miltime = miltime - rhs.miltime;
        pass = 1;
    }
    
   
    /*************************************************
    * Subtract two Time objects and return difference
    *   See examples in spec
    **************************************************/
   
    diff.hours = diff.miltime / 100;
    diff.minutes = diff.miltime % 100;
    //diff.miltime = 100 * diff.hours + diff.minutes;
    if(flipFlag != 1){
        diff.hours++;
        diff.minutes = diff.minutes - 60;
    } else if(pass != 1){
        //diff.hours++;
        if (rhs.hours > hours) {
            diff.hours++;
            if (rhs.miltime > miltime) {
                diff.minutes = diff.minutes - 60;
                escape = 1;
            }
        }
        if (escape == 0) {
            diff.minutes = 60 - (diff.minutes - 60);
        }
        
    }
    if (pass == 1) {
        diff.hours += 13;
        diff.miltime = diff.hours * 100 + diff.minutes;
    }
    if (diff.hours > 12) {
        diff.hours = diff.hours - 12;
        
    } else if (diff.hours == 0)
        diff.hours = 12;
    
    
    if ((diff.miltime >= 0000 && diff.miltime < 1200) || (diff.miltime >= 2400 && diff.miltime < 3600)) {
        diff.AMorPM = 'A';
    } else if((diff.miltime < 2400 && diff.miltime >= 1200) || ((diff.miltime >= 3600) && (diff.miltime < 4800))){
        diff.AMorPM = 'P';
    }
    return diff;
}

Time& Time::operator +=(const Time& rhs) {
   /* if (minutes + rhs.minutes >= 60)
        miltime = (rhs.miltime + miltime + 40) % 2400;
    else
        miltime = (rhs.miltime + miltime) % 2400;
    
    hours = miltime / 100;
    
    if (hours > 12) {
        hours = hours - 12;
    } else if (hours == 0)
        hours = 12;
    
    minutes = miltime % 100;
    if ((miltime >= 0000 && miltime < 1200) || (miltime >= 2400 && miltime < 3600)) {
        AMorPM = 'A';
    } else if((miltime < 2400 && miltime >= 1200) || ((miltime >= 3600) && (miltime < 4800))){
        AMorPM = 'P';
    }
    /**************************************************
    * Same as + operator, but modifies calling object
    *   and returns reference to calling object
    ***************************************************/
    *this = *this + rhs;
    return *this;
}

Time& Time::operator -=(const Time& rhs) {
    *this = *this - rhs;
    
    return *this;
    
}

// Increment operators--adds 1 minute to current time
Time& Time::operator++() {
    /*************************
    * Pre-increment operator
    **************************/
    if (minutes + 1 >= 60)
        miltime = (1 + miltime + 40) % 2400;
    else
        miltime = (1 + miltime) % 2400;
    
    hours = miltime / 100;
    
    if (hours > 12) {
        hours = hours - 12;
    } else if (hours == 0)
        hours = 12;
    
    minutes = miltime % 100;
    //determine time of day
    if ((miltime >= 0000 && miltime < 1200) || (miltime >= 2400 && miltime < 3600)) {
        AMorPM = 'A';
    } else if((miltime < 2400 && miltime >= 1200) || ((miltime >= 3600) && (miltime < 4800))){
        AMorPM = 'P';
    }
    /**************************************************
    * Same as + operator, but modifies calling object
    *   and returns reference to calling object
    ***************************************************/
    return *this;
}

Time Time::operator++(int) {
    Time newTime;
    newTime.miltime = miltime;
    newTime.minutes = minutes;
    newTime.hours = hours;
    newTime.AMorPM = AMorPM;
    if (minutes + 1 >= 60)
        miltime = (1 + miltime + 40) % 2400;
    else
        miltime = (1 + miltime) % 2400;
    
    hours = miltime / 100;
    
    if (hours > 12) {
        hours = hours - 12;
    } else if (hours == 0)
        hours = 12;
    
    minutes = miltime % 100;
    //determine time of day
    if ((miltime >= 0000 && miltime < 1200) || (miltime >= 2400 && miltime < 3600)) {
        AMorPM = 'A';
    } else if((miltime < 2400 && miltime >= 1200) || ((miltime >= 3600) && (miltime < 4800))){
        AMorPM = 'P';
    }
    /*************************
    * Post-increment operator
    **************************/
    return newTime;
}
/*** END OVERLOADED OPERATORS TO BE ADDED FOR PROGRAM 3 ***/
/*** DO NOT MODIFY CODE BELOW THIS LINE ***/
// Default constructor
Time::Time() : hours(0), minutes(0), miltime(0), AMorPM('A')
{}

// Parameterized constructor
Time::Time(unsigned h, unsigned m, char AP) : hours(h), minutes(m), AMorPM(AP) {
    miltime = 100 * h + m;

    /*** FIXED 10/11: ORIGINAL VERSION DID NOT CORRECTLY HANDLE 12 AM OR 12 PM ***/
    if (AP == 'P' && h != 12)
        miltime += 1200;
    else if (AP == 'A' && h == 12)
        miltime -= 1200;
}

// Set time data members
void Time::set(unsigned h, unsigned m, char AP) {
    hours = h;
    minutes = m;
    AMorPM = AP;
    miltime = 100 * h + m;
    if (AP == 'P')
        miltime += 1200;
}

// Print time to desired output stream
void Time::display(ostream& out) {
    out << hours << ':'
        << setw(2) << setfill('0') << minutes        // setw(2) forces minutes to be printed with 2 chars
        << ' ' << AMorPM << 'M';                    // setfill('0') adds leading 0 to minutes if needed
}

// Advance time by h hours, m minutes
// Use modulo arithmetic to ensure
//   1 <= hours <= 12, 0 <= minutes <= 59
/*** FIXED 10/11: ORIGINAL VERSION DIDN'T WORK FOR ALL CASES AND WAS FAR TOO CONVOLUTED ***/
/***  NEW VERSION DOES ALL MATH ON MILTIME AND THEN CORRECTS HOURS, MINUTES ***/
void Time::advance(unsigned h, unsigned m) {

    unsigned tempMT = h * 100 + m;        // Temporary miltime representing amount
                                                 //   of time to advance by, since math
                                                 //   is much easier using miltime!

    // If sum of minutes >= 60, need to account for extra hour added
    if (minutes + m >= 60)
        miltime = (miltime + tempMT + 40) % 2400;    // % 2400 ensures time between 0 & 2359
                                                             //   (since minutes adjustment guarantees
                                                             //    last two digits < 60)
    else
        miltime = (miltime + tempMT) % 2400;

    // Convert back from miltime to new hours/minutes
    hours = miltime / 100;

    // Special case 1: time in PM (other than 12 PM)
    if (hours > 12)
        hours -= 12;

    // Special case 2: 12:xx AM --> miltime / 100 = 0
    else if (hours == 0)
        hours = 12;

    minutes = miltime % 100;

    // Figure out if new time is in AM or PM
    AMorPM = (miltime < 1200 ? 'A' : 'P');
}

// Returns true if calling object is less than argument
bool Time::lessThan(const Time& rhs) {
    if (miltime < rhs.miltime)
        return true;
    else
        return false;
}
