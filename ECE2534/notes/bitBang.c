/*

mask - word with 1's in bit positions i'm interested in and 0's elsewhere

var x -
    set bits in x corresponding to 1's in mask, M.
        $ x | m
    clear the bits you want
        $ x & (~m)
    toggle the bits you want
        $ x ^ m   // exclusive or
        
    only keep bits correspond to 1's in m and 0 out the others
        $ x & m;
    
optimized compiles

    will move PORTG outside loop if only reading
    
        fix it by declaring it as volatile
        
*/

#include <stdio.h>

int main(){
    
    int a= 9;
    
    int b = 3;
    
    printf("hello. %d + %d = %d\n", a, b, a+b);
    char msg[17];
    sprintf(msg,"hellooo %d",a );
    
}

