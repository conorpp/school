
#include <iostream>
#include <stdlib.h>
using namespace std;

double rectangle(double width, double length);

int main(void ){
    double width, length, perimeter;
    
    cout << "Enter length and width \n";
    
    cin >> width >> length;
    
    perimeter = rectangle(width, length);
    
    cout << "length = " << length << endl;
    cout << "width = " << width << endl;
    cout << "perimeter = " << perimeter << endl;
    cout << "Area " << width * length << endl;
    
    return 0;
    
}
double rectangle(double width, double length){
    
    return (width*2 + length * 2);
}