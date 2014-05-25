
#include <stdio.h>

void strcopy(char *source, char *dest){
    
    while(*source){
        *(dest) = *(source);
        source++;
        dest++;
    }
}
//return index of of char in string
int indexOf(char *str,char c){
    int i = 0;
    while(*str){
        if (*str == c){
            return i;
        }
        i++;
        str++;
    }
    return -1;
}
int main(){
    char *str1 = "my name is conor";
    char *str2 = "my name is patri";
    strcopy(str1, str2);
    printf("%s", str2);
}