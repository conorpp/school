/*
 *  Conor Patrick.  1/28/2014.  ECE 2534.
 *  
    Functions for searching for a substring
    and reversing a string
*/
#include <stdio.h>

int search(char *str, char *substr){
    int startSub = 0;
    int index = 0;
    
    while (*str){

        while(*str == *substr){ //check if full match
            str++; substr++;
            startSub++;
            if (!*substr) return index;
        }
        // Reset to start of last search;
        str = str - startSub;
        substr = substr - startSub;
        startSub = 0;
    
        str++;
        index++;
    }
    return -1;
}

void reverse(char str[]){
    int start = 0;
    int end = length(str)-1;
    char holder;    // space for swapping char.
    
    while( end > start ){
        holder = str[end];
        // swap values
        str[end] = str[start];
        str[start] = holder;
        end--;
        start++;
    }
}
// returns length of string
int length(char *str){
    int i = 0;
    while(*str){
        str++;
        i++;
    }
    str = str - i;
    return i;
}


int main(){
    char name[] = "conor";
    
    reverse(name);
    
    printf("reversed is %s\n", name);
    
    k = search(name, "noc");
    
    printf("index of noc is %d\n",k);
}
