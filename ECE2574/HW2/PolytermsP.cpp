/*
    Conor Patrick
    03/04/2014
    
    Filename: Poly.cpp
    Implementation for Poly.h
*/

#include <iostream>
#include "PolytermsP.h"
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;

typedef Poly::Node Node;

/*
    Constructor.  Set head to NULL
*/
Poly::Poly(){
    this->head = NULL;
}
/*
    Copy Constructor.  Set head to NULL and copy over other Poly.
*/
Poly::Poly(const Poly& rhs){
    this->head = NULL;
    *this = rhs;
}
/*
    Destructor.  Deallocate all memory.
*/
Poly::~Poly(){
    Node *tmp = this->head;
    
    while(tmp != NULL){
        tmp = this->head;
        this->head = this->head->next;
        if (tmp != NULL) delete tmp;
        tmp = this->head;
    }
    this->head = NULL;
}

/*
    Adds a term with coefficient and exponent to end of linked list
    Use third argument to indicator whether or not to simplify as well.
*/
void Poly::append(CoefficientItem coef, int exp, int simple = 1){
    Node *conductor = this->head;
    Node *newNode = new Node;
    if (newNode == NULL) throw string("Memory allocation returned NULL");
    newNode->coefficient = coef;
    newNode->exponent = exp;
    if (conductor == NULL){
        this->head = newNode;
    }else{
        while(conductor->next != NULL)
            conductor = conductor->next;
        conductor->next = newNode;
    }
    newNode->next = NULL;
    if (simple)
        this->simplify();
}
/*
    Deletes a terms with matching exponent recursively.
*/
void Poly::del( int exp){
    Node *prior = NULL;
    Node *conductor = this->head;
    if (conductor == NULL)
        return;
    
    else if(conductor->next == NULL) {
        if (conductor->exponent == exp || conductor->coefficient == 0){
            if (conductor != NULL) delete conductor;
            this->head = NULL;
        }
        return;
    }
    while(conductor != NULL){
        if (conductor->exponent == exp || conductor->coefficient == 0){
            if (prior ){
                prior->next = conductor->next;
            }
            else this->head = conductor->next;
            if (conductor != NULL) delete conductor;
            this->del(exp);         // recurse
            return;
            
        }
        prior = conductor;
        if (conductor != NULL) conductor = conductor->next;
    }
}

/*
    Simplifies the polynomial by combining terms and deleting
    terms with 0 coefficient.
    Orders them at the end.
*/
void Poly::simplify(){
    if (this->head == NULL) return;
    Node *c1 = this->head;
    Node *c2 = c1->next;
    CoefficientItem cTotal = 0;
    Poly result;
    while(c1 != NULL){
        while(c2 != NULL){
            if (c1->exponent == c2->exponent){
                cTotal += c1->coefficient += c2->coefficient;
            }
            c2 = c2->next;
        }
        if (cTotal) {
            result.append(cTotal, c1->exponent, 0);
        }else
            if (c1->coefficient)
                result.append(c1->coefficient, c1->exponent, 0);

        cTotal = 0;
        this->del(c1->exponent);
        c1 = this->head;
        if (c1 != NULL ) c2 = c1->next;
    }
    result.order();
    *this = result; 
}
/*
    Orders the polynomial in order of descending exponents
*/
void Poly::order(){
    Node *c1 = this->head;
    Node *c2 = this->head;
    int c1e;
    CoefficientItem c1c;
    while(c1 != NULL){
        while(c2 != NULL){
            if (c2->exponent < c1->exponent){       //swap
                c1e = c1->exponent;
                c1c = c1->coefficient;
                c1->exponent = c2->exponent;
                c1->coefficient = c2->coefficient;
                c2->exponent = c1e;
                c2->coefficient = c1c;
            }
            c2 = c2->next;
        }
        c2 = this->head;
        c1 = c1 -> next;
    }
}

/*
    Prints out the polynomial to stdout for testing
*/
void Poly::print(){
    Node *c1 = this->head;
    while(c1 != NULL){
        if (c1->coefficient != 1 || c1->exponent == 0) cout << c1->coefficient;
        if (c1->exponent != 0 && c1->exponent != 1) cout << "X^" << c1->exponent;
        else if (c1->exponent == 1 ) cout << "X";
        c1 = c1->next;
        if (c1 != NULL) cout << " + ";
    }
    cout << endl;
}


/*
    Assignment operator overload for overwriting polynomials
*/
Poly& Poly::operator=(const Poly& rhs){
    Node *c1 = this->head;
    Node *c2 = rhs.head;
    
    while(this->head != NULL){
        this->head = this->head->next;
        if (c1 != NULL) delete c1;
        c1 = this->head;
    }
    
    while(c2 != NULL){
        this->append(c2->coefficient, c2->exponent,0);
        c2 = c2->next;
    }
    return *this;
}

/*
    + operator overload.  Adding polynomials
*/
const Poly Poly::operator+(const Poly& rhs) const{
    Poly result = *this;
    result += rhs;
    result.simplify();
    return result;
}

/*
    += operator overload. Adding polynomials
*/
Poly& Poly::operator+=(const Poly& rhs){
    if (rhs.head == NULL)
        return *this;
    Node *conductor = rhs.head;
    
    while(conductor != NULL){
        this->append( conductor->coefficient, conductor->exponent );
        conductor = conductor->next;
    }
    this->simplify();
    return *this;
}
/*
    Multiply left polynomial by right polynomial
*/
Poly& Poly::operator*=(const Poly& rhs){
    Poly result;
    Node *c1 = this->head;
    Node *c2 = rhs.head;
    while(c1 != NULL){
        while(c2 != NULL){
            result.append(
                c1->coefficient * c2->coefficient,
                c1->exponent + c2->exponent,
            0);
            c2 = c2->next;
        }
        c2 = rhs.head;
        c1 = c1->next;
    }
    result.simplify();
    *this = result;
}
/*
    Multiply two polynomials and return result
*/
const Poly Poly::operator*(const Poly& rhs) const{
    Poly result = *this;
    result *= rhs;
    return result;
}
/*
    Divides two polynomials and returns either the quotient or remainder.
    Indicate the type returned with 3rd arg.
*/
Poly Poly::divideBy(const Poly& rhs, int remainder = 0) {
    Poly div;
    Poly result;
    Poly zero;
    Poly negate;
    negate.append(-1,0);
    Poly tmp = *this;
    Node *c1 = tmp.head;
    Node *c2 = rhs.head;

    while( c1 != NULL ){        //this

        div.append(c1->coefficient / c2->coefficient,
                    c1->exponent - c2->exponent);
        result += div;
        div *= (rhs*negate);
        tmp += div;
        if (tmp.head == NULL || div.head == NULL || tmp.head->exponent < c2->exponent){
            break;
        }
        c1 = tmp.head;
        div = zero;
    }
    
    if (remainder) return tmp;
    else return result;
}
/*
    Division and remainder operator overloads
*/
Poly& Poly::operator/=(const Poly& rhs){
    *this = this->divideBy(rhs,0);
    return *this;
}

const Poly Poly::operator/( Poly& rhs) {
    Poly result = this->divideBy(rhs,0);
    return result;
}

const Poly Poly::operator%( Poly& rhs) {
    Poly result = this->divideBy(rhs,1);
    return result;
}

/*
    Writes polynomial terms to end of file for output.
*/
void Poly::writeTo(ofstream& file){
    if (!file.is_open()){
        cout << "Error writing to file because it's not open"<<endl;
        return;
    }
    Node *c1 = this->head;
    while(c1!=NULL){
        file << c1->coefficient <<" "<< c1->exponent << endl;
        c1 = c1->next;
    }
}



