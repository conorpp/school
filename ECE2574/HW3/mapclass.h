/* 

    Conor Patrick
    Homework 3
    mapclass.h - Map header
    
*/

#include <fstream>
#include "stackP.h"

class Map{
    
    public:
        Map();
        ~Map();
        
        void setOrigin(char origin);
        void Mapout( );
        void Recurse_Mapout( );
        bool flightTo(Node point);
        void addPoint(char p1, char p2, Price price);
        void mapPoint(char p1, char p2, Price price);
        void printMap();
        void printMap(std::ofstream &file);

    private:
        Stack maps;
        Stack points;
        int length;
        char origin;      
};