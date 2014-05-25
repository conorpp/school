/*
    Conor Patrick
    Homework 3
    stackP.cpp - Code implementation for stack
*/

#include <fstream>
#include <iostream>
#include <string>
#include "stackP.h"

using namespace std;



/*
    Constructor.  Set head to NULL
*/
Stack::Stack(){
    this->head = NULL;
    this->totalPrice = 0;
    this->destination = this->origin = '\0';
}
/*
    Copy Constructor.  Set head to NULL and copy over other Stack.
*/
Stack::Stack(const Stack& rhs){
    this->head = NULL;
    *this = rhs;
}

/*
    Destructor.  Deallocate all memory.
*/
Stack::~Stack(){
    Node *tmp = this->head;
    while(tmp != NULL){
        tmp = this->head;
        this->head = this->head->next;
        if (tmp != NULL) delete tmp;
        tmp = this->head;
    }
    this->head = NULL;
}

/*
    Assignment operator overload for overwriting stacks
*/
Stack& Stack::operator=(const Stack& rhs){
    Node *c1 = this->head;
    Node *c2 = rhs.head;
    
    while(this->head != NULL){              //erase lhs
        this->head = this->head->next;
        if (c1 != NULL) delete c1;
        c1 = this->head;
    }
    
    while(c2 != NULL){                      // add rhs components
        if (c2->city1) this->push(c2->city1, c2->city2, c2->price);
        else if (c2->stack != NULL) this->push(*(c2->stack));
        c2 = c2->next;
    }

    return *this;
}

/*
    Adds a term to end of stack by specifying values
*/
void Stack::push(char city1,char city2, Price p){
    Node *newNode = new Node(city1,city2, p);
    if (newNode == NULL) throw string("Memory allocation returned NULL");
    this->totalPrice += p;
    this->push(newNode);
}

/*
    Adds a term to end of stack by specifying a node;
*/
void Stack::push(Node node){
    this->push(node.city1, node.city2, node.price);
}

/*
    Adds a term with reference to a stack to stack.  For making nested stacks.
*/
void Stack::push(Stack &stack){
    
    Node *newNode = new Node();
    if (newNode == NULL) throw string("Memory allocation returned NULL");
    newNode->stack = &stack;
    
    this->push(newNode);
}

/*
    Adds a term to end of stack by specifying a node pointer.
*/
void Stack::push(Node *newNode){
    Node *conductor = this->head;    
    
    if (conductor == NULL){
        this->head = newNode;
    }else{
        while(conductor->next != NULL)
            conductor = conductor->next;
        conductor->next = newNode;
    }
    
    newNode->next = NULL;
    newNode->prior = conductor;
    
}


/*
    Removes last item from stack
*/
Node Stack::pop(){
    Node *c = this->head;
    Node *prior = NULL;
    if (c == NULL){
        Node fail('\0','\0',0);
        return (fail);
    }
    while (c->next != NULL){
        prior = c;
        c = c->next;
    }
    Node tmp = *c;
    this->totalPrice -= c->price;
    if (prior != NULL) prior->next = NULL;
    else this->head = NULL;
    if (c != NULL) delete c;
    
    return tmp;
}

/*
    Returns last item without dropping it
*/
Node Stack::peak(){
    Node tmp = this->pop();
    this->push(tmp);
    return tmp;
}

void Stack:: print(){
    Node *c = this->head;

    while(c != NULL){
        cout<< "  " << c->city1
            << "  " << c->price << endl;
        c = c->next;
    }
}
/*
    Write output to a stdout
*/
void Stack:: printRoute(){
    Node *c = this->head;
    
    cout << this->peak().city2 << "              ";
    string points;
    string pad = "   ";
    int padding = 20;
    while(c != NULL){
        padding--;
        points += c->city1;
        points += ", ";
        padding -= 2;

        if (c->next == NULL){
            points += c->city2;
            padding--;
        }
        
        c = c->next;
        if (padding < 0) padding = 0;
    }
    while(padding--){
        pad += " ";
    }
    cout << points << pad << '$' << this->totalPrice << endl;
}


/*
    Write output to a file
*/
void Stack:: printRoute(ofstream &file){
    Node *c = this->head;
    
    file << this->peak().city2 << "              ";
    string points;
    string pad = "   ";
    int padding = 20;
    while(c != NULL){
        padding--;
        points += c->city1;
        points += ", ";
        padding -= 2;

        if (c->next == NULL){
            points += c->city2;
            padding--;
        }
        
        c = c->next;
        if (padding < 0) padding = 0;
    }
    while(padding--){
        pad += " ";
    }
    file << points << pad << '$' << this->totalPrice << endl;
}

/*
    Print to stdout for debugging stack class
*/
void Stack:: printStacks(){
    Node *c = this->head;
    while(c != NULL){
        if (c->stack != NULL){
            c->stack->print();
        }
        c = c->next;
    }
}

