/*

*/

#include <iostream>
using namespace std;

class List{

private:
    struct node{
        int data;
        node *poinnextter;
    }
    node conducter;
    node start;
    long int length;
public:
    List(){
        this.conducter = this.node;
        this.conducter.next = 0;
        this.start = this.conducter;
        printf("initialized the list\n");
    }
    void add(int listItem){
        while(this.conducter.next){
            this.conducter = this.conducter.next;
        }
        this.conducter.next = new node;
        this.conducter = this.conducter.next;
        this.conducter.data = listItem;
        this.conducter.next = 0;
    }
    void print(){
        this.conducter = this.start;
        while(this.conducter.next){
            this.conducter
        }
    }
    
};

int main(){
    
}