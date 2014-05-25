/*
    Conor Patrick, HW5
    04/22/2014
    mergesort.h
        Header for mergesort algorithm
*/
#ifndef IOSTREAM_H
    #include <iostream>
    #define IOSTREAM_H
#endif
#ifndef FSTREAM_H
    #include <fstream>
    #define FSTREAM_H
#endif

using namespace std;

class item;
class huff;
class huffman;
/*
    ADT for letter and frequency pair
*/
class item{
    friend class mergeSort;
    friend class huff;
    friend class huffman;
    public:
        item(const char a='\0',const int v=0){ this->letter = a; this->freq = v;  }
        item(const item &orig){ this->letter = orig.letter, this->freq = orig.freq; }
        
        item operator=(const item &orig){ this->letter = orig.letter, this->freq = orig.freq; return *this;}
        bool operator<(const item &orig);
        bool operator>(const item &orig);
        bool operator>=(const item &orig);
        bool operator<=(const item &orig);
        
        bool operator<(const huff &orig);
        bool operator>(const huff &orig);
        bool operator>=(const huff &orig);
        bool operator<=(const huff &orig);
        
        void print(){cout << (this->letter ? this->letter : ' ')<<(this->letter ? ':' : ' ')<<this->freq;}
    private:
        char letter;
        int freq;
};

/*
    Algorithm for sorting items via mergesort
*/
class mergeSort{
    
    public:

        mergeSort(item *p, int s);
        ~mergeSort();
        
        void print();
        void write(ofstream &file);
        
        void sort(item items[], int start, int end);
        void merge(item items[], int start, int mid, int end);
        
        item* data(){return this->_data;}
        int size(){return this->_size;}
        
    private:
        item *_data;
        item *tmp;
        int _size;
};

