/*
    Conor Patrick
    
    HW6
    find3 - returns 1 if int has 3 consecutive ones
    reverse - reverses the bits of an int
*/
#include <stdio.h>

unsigned int find3(unsigned int x){
    
    int three = 0;
    int l = sizeof(int)*8;
    while(l-- > 0){
        if ((1 << l) & x){
            three++;
            if (three >= 3) return 1;
        }else
            three = 0;
    }
    return 0;
}
// Reverses the bits in x
unsigned int reverse(unsigned int x){
    int l = sizeof(int)*8;
    int start = 0;
    int startBit, lastBit, holder;
    
    while(l--){
        startBit = 1 << start;
        lastBit = 1 << l;
        holder = x;
        if (x & lastBit){         //set the bit
            x = x | startBit;
        }else{                      //clear the bit
            x = x & (~startBit);   
        }
        if (holder & startBit){         // swap the other side
            x = x | lastBit;
        }else{
            x = x & (~lastBit);
        }
        start++;
        if (start >= l) break;
    }
    return x;
}

// Testing
int main(){
    int find2 = 0x66;
    int find33 = 0xFF;
    find2 = find3(find2);
    find33 = find3(find33);
    printf("\ntwo 1s: %d\nthree 1s: %d\n", find2, find33);
    
    int rev = 0x6;
    rev = reverse(rev);
    printf("reversed of 0x6 : 0x%x\n\n", rev);
}
