/*
    Conor Patrick
    HW4 - bitbanger function and modified int2str with base
    2/2/2014
*/

#include <stdio.h>


// converts an int with base, base, to a string
void int2str(unsigned int num, int base, char *str){
    int digit;
    
    int pos = places(num, base); //number of digits
    int l = length(str);
    if (pos > l){ //check if enough space
        printf("ERROR: not enough space in str for conversion");
        return;
    }
    int copy = pos+1;
    while(pos >= 0){
        digit = num / EXP(base, pos);   //get digit
        num = num - EXP(base, pos)*digit; //remove that digit
        *str = digit + 0x30;
        pos--;
        str++;
    }
}

// returns 1 is ith bit of x is 1, 0 otherwise
int getBit(int x, int i){
    int banger = EXP(2, i);
    if (x & banger) return 1;
    else return 0;
}

// returns how many digits in integer with base base
int places(unsigned int num, int base){
    int i = 0;
    while (num >= 1){
        num = num/base;
        i++;
    }
    return i;
}


// returns x^y
int EXP(int x, int y){
    int power = 1;
    while (y--)
        power = power*x;
    
    return power;
}

// returns length of string
int length(char *str){
    int i = 0;
    while(*str){
        str++;
        i++;
    }
    return i;
}

int main(){
    int dec = 23;
    int base = 5;
    char str[7] = "   ";
    int2str(dec, base, str);
    
    printf("the int %d was converted to str %s (base %d)\n",
           dec, str, base);
    
    int hex = 0x8;
    int i = 3;
    int banged = getBit(hex, i);
    printf("the %dth bit of %d is %d\n", i, hex, banged);
    
    return 0;
}