/*
    Conor Patrick, HW5
    04/22/2014
    huffman.h
        Header file for Huffman binary tree structure
*/

#include "mergesort.h"
#ifndef IOSTREAM_H
    #include <iostream>
    #define IOSTREAM_H
#endif
#ifndef STRING_H
    #include <string>
    #define STRING_H
#endif
#ifndef FSTREAM_H
    #include <fstream>
    #define FSTREAM_H
#endif


using namespace std;

/*
    ADT for binary tree
    Huff ADT inherits from item ADT
*/
class huff : item{
    friend class huffman;
    friend class item;
    public:
        huff(item *d);
        huff(huff *d);
        huff();
        ~huff();
        
        void add(huff *i1, huff *i2);
        void print(){ cout << (this->letter ? this->letter : '\0')<<(this->letter ? ':' : '\0')<<this->freq; }

        huff* zeroBranch() const;
        huff* oneBranch() const;        

        bool operator<(const huff &orig);
        bool operator>(const huff &orig);
        bool operator>=(const huff &orig);
        bool operator<=(const huff &orig);
        
        bool operator<(const item &orig);
        bool operator>(const item &orig);
        bool operator>=(const item &orig);
        bool operator<=(const item &orig);
        
    private:
        huff *left;
        huff *right;
        bool sorted;
};
/*
    Structure for serializing binary tree
*/
class huffman{
    public:
        huffman(item *ptr, int size);
        ~huffman();
        void serialize();
        void sort();
        
        void addToTree();
        
        void print();
        int _print_(huff *p, int goallevel, int currentlevel, int spaceMult);
        
        void write(ofstream &file);
        void _write_(huff *r, ofstream &file, string path);
        
        void traverse();
        void _traverse_(huff *r);
        huff* getVal(string str);

    private:
        huff *root;
        int size;
        int sorted;
        int maxLevels;
        
};

