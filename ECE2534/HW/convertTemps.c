/*
    Conor Patrick
    
    Convert fairenheight to celsius FtoC()
    return max val in an array maxarray()
*/
#include <stdio.h>

// convert F to C without floating point arithmetic
unsigned int FtoC(unsigned int F){
    return (F-32)*10/18;
}

// return max value in a array with specified size
int maxarray(int array[ ], int size){
    int max = 0;
    while(size--)
        if (array[size] > max) max = array[size];
    return max;   
}

//testing
int main(){
    unsigned int fair = 300;
    unsigned int cel = FtoC(fair);
    printf("%d F == %d C\n", fair, cel);
    
    int const size = 5;
    int arr[] = {1,55,340,22,-6};
    int max = maxarray(arr, size);
    printf("max of array: %d\n", max);
}