/*
    Conor Patrick, HW5
    04/22/2014
    huffman.cpp
        Implementation of huffman.h (binary tree)
*/

#include "huffman.h"
#ifndef FSTREAM_H
    #include <fstream>
    #define FSTREAM_H
#endif

/*
    Set legs to NULL to be in a known state. Init with a node.
*/
huff::huff(huff *d){
    this->left = d->left;
    this->right = d->right;
    this->letter = d->letter;
    this->freq = d->freq;
    this->sorted = d->sorted;
}
huff::huff(item *d){
    this->left = NULL;
    this->right = NULL;
    this->letter = d->letter;
    this->freq = d->freq;
    this->sorted = false;
}
/*
    Set legs to NULL to be in a known state. Init with no data.
*/
huff::huff(){
    this->left = NULL;
    this->right = NULL;
    this->letter = '\0';
    this->freq = 0;
    this->sorted = false;
}

/*
    Clean up allocated memory
*/
huff::~huff(){
    if (this->right != NULL){
        //delete this->right;
        this->right = NULL;
    }
    if (this->left != NULL){
        //delete this->left;
        this->left = NULL;
    }
}

/*
    Add an item to a huff node or initialize a huff node with two legs
*/
void huff::add(huff *i1, huff *i2){
    if (*i1 < *i2){
        this->left = new huff(i1);
        this->right = new huff(i2);
    }else{
        this->left =  new huff(i2);
        this->right = new huff(i1);
    }
    if (this->left == NULL || this->right == NULL)
        throw string("new returned NULL in huff::add");

    this->letter = '\0';
    this->freq = i1->freq + i2->freq;
}


/* allocate root for data */
huffman::huffman(item *ptr, int size){
    this->size = size;
    this->root = new huff[size];
    if (this->root == NULL)
        throw (string("new returned NULL in huffman::huffman"));
    for (int i=0; i<size; i++){
        this->root[i] = ( huff( &(ptr[i]) )) ;
    }
    
    this->maxLevels = 50;
    this->sorted = 0;
    this->serialize();
}

/* Cleanup */
huffman::~huffman(){
    if (this->root != NULL) delete[] this->root;
}

/*
    Sort raw array simplify it into a binary tree
*/
void huffman::serialize(){
    /* I would assume data is sorted in the beginning but I'm leaving this commented just in case it's not.  */
    // if (this->size > 1){    
    //     this->addToTree();
    // }
    
    /* continuously sort until there's just one root node */
    while((this->size - this->sorted) > 1){
        this->sort();
        this->addToTree();
    }
    
}



/*
    Subroutine for huffman::serialize()
*/
void huffman::sort(){
    int s = this->size;
    for (int i = 0; i<s; i++){
        if (this->root[i].sorted) continue;
        for (int j = i+1; j<s; j++){
            if (this->root[j].sorted) continue;
            /* swap positions */
            if (this->root[i] > this->root[j]){     
                huff tmp (this->root[i]);
                this->root[i] = this->root[j];
                this->root[j] = tmp;
            }
        }
    }
    
}
/*   combine first two elements in data array   */
void huffman::addToTree(){
    int place1 = -1, place2 = -1;
    for (int i = 0; i<this->size; i++){     // Find first two unsorted elements to merge
        if (this->root[i].sorted) continue;
        else if (place1 == -1){
            place1 = i;
        }else if (place2 == -1){
            place2 = i;
            break;
        }
    }
    if (place1<0 || place2<0)
        throw string("all elements marked as sorted before simplified to root.");
    this->root[place1].add(  &(this->root[place1]), &(this->root[place2]) );
    this->root[place2].sorted = true;       // mark element as sorted to not merge with it again.
    this->sorted++;
}
/*
    Retreives the value for a corresponding binary string e.g. "0110"
*/
huff* huffman::getVal(string str){
    int i = 0;
    huff *p = &this->root[0];
    for (i=0;i<str.length();i++){
        if (p->right == NULL || p->left == NULL)
            throw (string("That binary path is illegal: ")+str);
        if (str[i] == '1'){
            p = p->oneBranch();
        }else if (str[i] == '0'){
            p = p->zeroBranch();
        }else
            throw (string("You must specify a binary path: ")+str);
                
    }
    return p;
}
/*
    Returns a pointer to the '1' path branch for a given node.
    This is the rule-set that makes the binary paths zero optimized.
*/
huff* huff::oneBranch()const{
    /*
        If node points to two more branches OR
        if node points to two terminating nodes, assign 1 to smaller freq
        THEN assign 1 to smaller branch/node
    */
    if (   (!this->left->letter && !this->right->letter) ||
           ( this->left->letter &&  this->right->letter)   ){   
        if (this->left->freq < this->right->letter)
            return this->left;
        else
            return this->right;
    }

    /*
        If node points to one terminating node and one branch, assign 1 to terminating node
    */
    if (this->left->letter)
        return this->left;
    else
        return this->right;
    
}
/*
    Returns a pointer to the '0' path branch for a given node
*/
huff* huff::zeroBranch()const{
    return this->oneBranch() == this->left ? this->right : this->left;    
}

/*
    Print out binary tree using recursive subroutine (like a tree).  Not linear
*/
void huffman::print(){
    int leftover1=0, leftover2=0;
    for (int i=0; i<this->maxLevels; i++){
        for (int j=0; j < this->size; j++){
            if (this->root[j].sorted) continue;
            leftover1 = this->_print_(&this->root[j], i, 0, (j+1));
            if (leftover1) leftover2 = 1;
        }
        if (leftover2) cout << endl;
        leftover2 = 0;
    }
    cout << endl;
}
/*
    Subroutine for print recursion
*/
int huffman::_print_(huff *p, int goallevel, int currentlevel, int spaceMult=1){
    if (p == NULL) return 0;
        int leftover1, leftover2;
        int spaces;
         //cout << "goal: "<< goallevel<< "   current: "<< currentlevel<<endl;
        if (goallevel == currentlevel){
            if (p->left == NULL && p->right == NULL){
                spaces = 15;
            }else spaces = 7;
                    
            for (int i=maxLevels*spaceMult; i>goallevel; i-=spaces)
                cout << " ";
            p->print();
                
        }else{
            leftover2=  this->_print_(p->left, goallevel, currentlevel+1, spaceMult);
            leftover1 = this->_print_(p->right, goallevel, currentlevel+1, spaceMult);
        }
    return (leftover1 || leftover2 ? 1 : 0);
}

/* prints out binary tree linearly */
void huffman::traverse(){
    this->_traverse_(&this->root[0]);
    cout << endl;
}

/* recursive subroutine for huffman::traverse */
void huffman::_traverse_(huff *r){
    if (r == NULL) return;
    r->print();
    cout << "  ";
    this->_traverse_(r->left);
    this->_traverse_(r->right);
}
/*
    Write each charater to a file with huffman code
*/
void huffman::write(ofstream &file){
    this->_write_(&this->root[0], file, string(""));
}

/* recursive subroutine for huffman::write */
void huffman::_write_(huff *r, ofstream &file, string path){
    if (r->letter){
        file << r->letter << "  " << path << endl;
    }else{
        this->_write_( r->zeroBranch() , file, path+'0');
        this->_write_( r->oneBranch(), file, path+'1');
    }
}


/* operator overloads for huff */
bool huff::operator<(const huff &orig){ return (this->freq < orig.freq);}
bool huff::operator>(const huff &orig){ return (this->freq > orig.freq);}
bool huff::operator>=(const huff &orig){ return (this->freq >= orig.freq);}
bool huff::operator<=(const huff &orig){ return (this->freq <= orig.freq);}
        

bool item::operator<(const huff &orig){
    return (this->freq < orig.freq);
}
bool item::operator>(const huff &orig){
    return (this->freq > orig.freq);
}
bool item::operator>=(const huff &orig){
    return (this->freq >= orig.freq);
}
bool item::operator<=(const huff &orig){
    return (this->freq <= orig.freq);
}



