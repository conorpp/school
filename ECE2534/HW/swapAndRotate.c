/*
Functions for swaping 16 bits in integer
    and bit shifting
    
    Conor Patrick
    Micro
    2/4/2014
*/

#include <stdio.h>

// Swap first half bits with second half bits
int swap(int x){
    
    unsigned int firstHalf =  0xFFFF0000 & x;
    unsigned int secondHalf = 0x0000FFFF & x;
    unsigned int shifts = 16;
    firstHalf = firstHalf >> shifts;
    secondHalf = secondHalf << shifts;
    return (firstHalf | secondHalf);
}

// circular shift left for x
unsigned int rotateLeft(unsigned int x){
    unsigned int lastBit = 0x80000000 & x;
    lastBit = lastBit >> 31;
    x = x << 1;
    return (x | lastBit);
}

// testing the functions
int main(){
    
    unsigned int swapMe = 0x12345678;
    swapMe = swap(swapMe);
    
    unsigned int rotateMe = 0x88888888;
    rotateMe = rotateLeft(rotateMe);
    
    printf("swapped: %x\nrotated: %x\n", swapMe, rotateMe);
    
    return 0;
}