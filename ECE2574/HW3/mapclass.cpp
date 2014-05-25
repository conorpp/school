/* 

    Conor Patrick
    Homework 3
    mapclass.h - Map implementation
    
*/

#include <fstream>
#include <iostream>
#include "mapclass.h"


using namespace std;
/*
    Constuctor/Destructors
*/
Map::Map(){
    this->length = 0;
    this->origin = '\0';
}

Map::~Map(){
    
}

/*
    Set the origin city
*/
void Map::setOrigin(char origin){
    this->origin = origin;
}
/*
    Add two coordinating cities to a stack unsorted.
*/
void Map::addPoint(char p1, char p2, Price price){
    this->points.push(p1, p2, price);
    this->length++;
}
/*
    Makes a recursive call for a number of points
*/
void Map::Mapout(){
    if (this->length >= 0){
        int l = this->length;
        while(l--){
            this->Recurse_Mapout();
        }
    }
}
/*
    Recursively organizes a point into a map based on origin.
*/
void Map::Recurse_Mapout(){
    Node tmp = this->points.pop();

    if (tmp.city1 == this->origin || this->flightTo(tmp)){
        mapPoint(tmp.city1, tmp.city2, tmp.price);
    }else if( !tmp.city1 ){
        return;
    }else{
        Recurse_Mapout();
        this->points.push(tmp);
    }
}
/*
    Indicate whether if a flight to a point exists in map
*/
bool Map::flightTo(Node point){

    Node *c = this->maps.head;
    while(c != NULL){
        if (c->stack != NULL){
            if (c->stack->peak().city2 == point.city1){
                return true;
            }
        }
        c = c->next;
    }
    return false;
}

/*
    Add two coordinating cities to the map.
*/
void Map::mapPoint(char p1, char p2, Price price){
    Stack *map = new Stack;
    if (p1 == this->origin){
        
        map->push(p1,p2,price);
        map->origin = p1;
        
        this->maps.push(*map);
        
        return;
    }
    Node *c = this->maps.head;
    
    while(c != NULL){
        if (c->stack->peak().city2 == p1 && c->stack != NULL){
            *map = *(c->stack);
            map->push(p1,p2,price);
            this->maps.push(*map);
            return;
        }
        c = c->next;
    }
    
    map->push(p1,p2,price);
    this->maps.push(*map);
}


/*
    Print to stdout
*/
void Map::printMap(){
    cout << "Destination    Flight Route from "<< this->origin<<"    Price "<<endl;
    Node *c = this->maps.head;
    while(c != NULL){
        c->stack->printRoute();
        c = c->next;
    }
}

/*
    Print to file
*/
void Map::printMap(ofstream &file){
    file << "Destination    Flight Route from "<< this->origin<<"    Price "<<endl;
    Node *c = this->maps.head;
    while(c != NULL){
        c->stack->printRoute(file);
        c = c->next;
    }
};







