/*
    Conor Patrick
    
    simulate.h
    
    The class for maintaining an event list
*/

#include <string>
#include <stdlib.h>
#include <queue>
#include <fstream>
#include "simulateTypes.h"

class EventList{
    private:
        BankStat tellers;
        BankStat customers;
        BankStat avgTimeInBank;
        BankStat maxTimeInBank;
        BankStat avgTimeWaiting;
        BankStat maxTimeWaiting;
        BankStat avgLineLength;
        BankStat maxLineLength;
        std::string arrival;
        std::string departure;
        std::string simulationStarted;
    public:
        EventList();
        ~EventList();
        
        std::queue <Arrival> line;
        std::queue <Event> events;
        
        void add();
        void printLine();
        void printEvents(std::queue<Event> &q);
        void printEvents(std::ofstream &file);
        void printStats(std::ofstream &file);
        void printStats();
        void findEvents();
        void sortEvents(std::queue<Event> &temp);
        BankStat getAvgLineLength();
};


