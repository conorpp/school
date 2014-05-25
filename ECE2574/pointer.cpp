
#include <iostream>
using namespace std;

int main(void ){
    
    int i = 27;
    int *p;
    
    p = &i;
    
    cout << "the val of i is " << i << endl;
   // cout << "The val of p is " << p << (intptr_t) p << endl;
    cout << "the val of *p is " << *p << endl;
    
    *p = 5;
    cout << "the val of i is now " << i << endl;
}