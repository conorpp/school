
/*
 *  Conor Patrick
 *  Data Stuctures
 *  2/3/2014
 *
    Outputs a string in reverse order and exits.
*/

#include <iostream>
using namespace std;


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

//overwrites string with reverse given a pointer to it.
void reverse(char *str){
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

int main(){
    
    char str[] = "";
    
    cout << "Enter a string to be reversed: " << endl;
    
    cin >> str;
    
    reverse(str);
    
    cout << "Reversed: " << str << endl;
}