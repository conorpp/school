#include <stdio.h>

int strLength(char *string){
    int i = 0;
    while (*string){
        i++;
        string++;
    }
    return i;
}

int main(){
    /*
        An array of chars.  will be 6 elements long because of null terminator, 0.
    */
    char msg[] = "hello";
    
    /*
        This is the same thing
    */
    char *msg1 = "hello";
    
    
    int l = strLength(msg1);
    
    printf( "the length is %d \n", l );
}



