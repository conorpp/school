
#include <iostream>
#include <string>
#include <math.h>
#include "simulate.h"

using namespace std;


/*
    Constructor / destructor
*/
EventList::EventList(){
    this->tellers = 1;
    this->customers = this->avgTimeInBank =
        this->maxTimeInBank = this->avgTimeWaiting =
        this->maxTimeWaiting  = this->avgLineLength =
        this->maxLineLength = 0;
    this->arrival = string("Arrival");
    this->departure = string("Departure");
    this->simulationStarted = string("Simulation started");
}
EventList::~EventList(){
    
}
/*
    Prints arrival/departures to stdout for debugging
*/
void EventList::printLine(){
    for (int i = this->line.size(); i>0; i--){
        Arrival a = this->line.front();
        a.print();
        this->line.pop();
        this->line.push(a);
    }
}

/*
    Prints events to a file
*/
void EventList::printEvents(queue<Event> &s){
    for (int i = s.size(); i>0; i--){
        Event a = s.front();
        a.print();
        s.pop();
        s.push(a);
    }
}
/*
    Prints stats to stdout
*/
void EventList::printStats(){
    cout
    << "Total number of customers = " << this->customers << endl
    << "Average amount of time in the bank = " << this->avgTimeInBank << " minutes" <<endl
    << "Maximum amount of time in the bank = " << this->maxTimeInBank << " minutes" <<endl
    << "Average amount of time waiting = " << this->avgTimeWaiting << " minutes" <<endl
    << "Maximum amount of time waiting = " << this->maxTimeWaiting << " minutes" <<endl
    << "Average length of line = " << this->getAvgLineLength() << " customers" <<endl
    << "Maximum length of line = " << this->maxLineLength << " customers" <<endl;
}
/*
    Prints events to a file
*/
void EventList::printEvents(ofstream &file){
    for (int i = this->events.size(); i>0; i--){
        Event a = this->events.front();
        a.print(file);
        this->events.pop();
        this->events.push(a);
    }
}
/*
    Prints stats to a file
*/
void EventList::printStats(ofstream &file){
    file
    << "Total number of customers = " << this->customers << endl
    << "Average amount of time in the bank = " << this->avgTimeInBank << " minutes" <<endl
    << "Maximum amount of time in the bank = " << this->maxTimeInBank << " minutes" <<endl
    << "Average amount of time waiting = " << this->avgTimeWaiting << " minutes" <<endl
    << "Maximum amount of time waiting = " << this->maxTimeWaiting << " minutes" <<endl
    << "Average length of line = " << this->getAvgLineLength() << " customers" <<endl
    << "Maximum length of line = " << this->maxLineLength << " customers" <<endl;
}
/*
    Generates the event list and calculates stats from
    the arrival and departure list.
*/
void EventList::findEvents(){
    if (!this->line.size()) return;
    Event *ptr = new Event(0, this->simulationStarted);
    queue<Event> temp;
    temp.push(*ptr);
    BankStat start , end, departOffset, arrivalOffset, wait, lineLength, diff;
    departOffset = arrivalOffset = end = start = wait = lineLength = 0;
    
    while(!this->line.empty()){
        Arrival a = line.front();
        line.pop();
        ptr = new Event(a.arrival, (this->arrival));
        temp.push(*ptr);
        
        
        start = a.arrival;
        if (start < end){
            wait = end - start;
            start = end;
            lineLength++;
        }else{
            wait = 0;
            lineLength = 0;
        }
        end = start + a.transaction;
        ptr = new Event(end, this->departure);
        temp.push(*ptr);
        diff = ((end - start) + wait);
        this->customers++;
        this->avgTimeInBank += diff;
        this->avgTimeWaiting += wait;
        this->maxLineLength = (this->maxLineLength < lineLength) ? lineLength : this->maxLineLength;
        this->maxTimeWaiting = (this->maxTimeWaiting < wait) ? wait : this->maxTimeWaiting;
        this->maxTimeInBank = (this->maxTimeInBank < diff) ? diff : this->maxTimeInBank;
    }
    this->avgTimeInBank /= this->customers;
    this->avgTimeWaiting /= this->customers;
        
    this->sortEvents(temp);
    
}

/*
    Sorts the event list chronologically
*/
void EventList::sortEvents(queue<Event> &temp){
    Event Min(_MAX_TIME, "__start__");
    Event _Min = Min;
    Event holder;
    while (!temp.empty()){
        for (int i = temp.size(); i>0; i--){
       
            holder = temp.front();
            temp.pop();
            if(Min >= holder){
                if (Min != _Min) temp.push(Min);
                Min = holder;
            }else{
                temp.push(holder);
            }
            
        }
        this->events.push(Min);
        Min = _Min;
    }
    
}
/*
    Calculates the average line length for the event list
*/
BankStat EventList::getAvgLineLength(){
    Event holder;
    BankStat avg = 0;
    int l = 0;
    int lT;
    int sT = lT = this->events.front().t;
    if (!this->events.size()){
        return 0;
    }
    for (int i = this->events.size(); i>0; i--){

        holder = this->events.front();
        this->events.pop();
        this->events.push( holder );
        if (holder.event == this->simulationStarted){
            sT = lT = this->events.front().t;
            continue;
        }
        avg += ((holder.t - sT) * l);
        sT = holder.t;
        
        if (holder.event == this->arrival){
            l++;
        }else if (holder.event == this->departure){
            l--;
        }
        l = (l >= 0) ? l : 0;

    }
    return floor(100 * (avg / (sT - lT)) + 0.5f) / 100;
}



