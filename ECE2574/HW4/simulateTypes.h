/*
    Conor Patrick
    
    simulateTypes.h
    
    The classes and types used for simulate.h
*/


typedef float BankStat;
#define _MAX_TIME 99999999

/*
    Data item for loading arrival and transaction pairs
*/
class Arrival{
    public:
        Arrival(std::string _arrival, std::string _transaction){
            this->arrival = atoi(_arrival.c_str());
            this->transaction = atoi(_transaction.c_str());
        }
        Arrival(const Arrival &a){
            this->arrival = a.arrival;
            this->transaction = a.transaction;
        }
        ~Arrival(){}
        void print(){  std::cout<<arrival<< " "<<transaction<<std::endl;  }
        int arrival;
        int transaction;
    private:
};

/*
    The data item for events
*/
class Event{
    public:
        Event(int _t = 0, const char *_event = " "){
            this->t = _t;
            std::string s(_event);
            this->event = s;

        }
        Event(int _t, std::string _event){
            this->t = _t;
            this->event = _event;

        }
        Event(const Event &a){
            this->t = a.t;
            this->event = a.event;
        }
        Event& operator=(const Event& orig){
            this->t = orig.t;
            this->event = orig.event; 
        }
        ~Event(){}
        void print(){  std::cout<< t<< ": "<< event<<std::endl;  }
        void print(std::ofstream &file){  file<< t<< ": "<< event<<std::endl;  }
        
        bool operator>(const Event& orig) const{ return (this->t > orig.t); }
        bool operator>=(const Event& orig) const{ return (this->t >= orig.t); }
        bool operator<=(const Event& orig) const{ return (this->t <= orig.t); }
        bool operator<(const Event& orig) const{ return (this->t < orig.t); }
        bool operator==(const Event& orig) const{ return (this->t == orig.t
                                                && this->event == orig.event); }
        bool operator!=(const Event& orig) const{ return !(*this == orig); }
        
        int t;
        std::string event;
};




