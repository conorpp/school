/*
    Two functions that convert ascii to ints
*/
#include <stdio.h>

// convert string to int
unsigned int str2dec(char *str){
    int l = length(str) - 1;
    int total = 0;
    while(*str){
        total = total+ ((*str - '0') * pwrTen(l));
        l--;
        str++;
    }
    return total;
}

// converts an int to a string
void int2str(int num,char *str){
    int digit;
    int pos = places(num);
    
    while(pos >= 0){
        digit = num / pwrTen(pos);
        num = num - pwrTen(pos)*digit;
        *str = digit + 0x30;
        pos--;
        str++;
    }
    
}


// returns 10^x
int pwrTen(int x){
    int power = 1;
    while (x--){
        power = power*10;
    }
    return power;
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

// returns how
int places(int num){
    //float f = (float) num;
    int i = 0;
    while (num > 10){
        num = num/10;
        i++;
    }
    return i;
}

int main(){
    char *num = "2011";
    int realNum;
    
    realNum = str2dec(num);
    
    printf("the int is %d\n",realNum);
    
    char strNum[4];
    
    int2str(realNum, strNum);
    printf("the converted back string is %s \n", strNum);
}
