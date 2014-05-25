/*
    Conor Patrick
    Homework 3
    stackP.h - header for stack
*/

#include <fstream>

typedef int Price;
class Stack;
class Map;

struct Node{
    Node(char city1 = '\0',char city2 = '\0', Price p=0,Stack *s = NULL){
        this->city1 = city1;
        this->city2 = city2;  this->price = p;
        this->next = this->prior = NULL; this->stack = s;
    }
    bool operator==(const Node& o) const{
        return (this->city1 == o.city1 && this->city2 == o.city2 && this->price == o.price);
    }
    char city1;
    char city2;
    Price price;
    Stack *stack;
    Node *next;
    Node *prior;
};

class Stack{
    
    friend class Map;
    
    public:
        
        Stack();
        ~Stack();
        Stack(const Stack& rhs);
        
        void push(char city1, char city2, Price p);
        void push(Node newNode);
        void push(Stack &stack);
        void push(Node *newNode);
        Node pop();
        Node peak();
        
        void print();
        void printStacks();
        void printRoute();
        void printRoute(std::ofstream &file);
        Stack& operator = (const Stack& rhs);
        
    private:
        Node *head;
        int totalPrice;
        char destination;
        char origin;
};


