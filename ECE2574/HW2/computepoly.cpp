/*
    Conor Patrick
    03/04/2014
    
    Filename: Poly.h
    Client code for polynomial computation.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "PolytermsP.h"
using namespace std;


int main(int argc, char *argv[]){
    try{
        string error;
        /*
            Check if valid arguments
        */
        if (argc < 3){
            error = "You must provide two arguments: \n ./computepoly <input-file> <output-file>\n";
            throw error;
        }
        /*
            Prepare for input
        */
        string word;
        ifstream ifile(argv[1]);
        if (!ifile.is_open()){
            error = "Input file \"";
            error += argv[1];
            error += "\" was not found.";
            throw error;
        }
        CoefficientItem coef;
        int exp;
        Poly a;
        Poly b;
        Poly *c= &a;
        
        /*
            Read the polynomials
        */ 
        while(ifile >> word){
            if (word.length() == 3){
                ifile >> word;
                c = &b;
            }
            coef = atoi(word.c_str());
            if (coef == 0){
                error = "A zero coefficient found in ";
                error += argv[1];
                throw error;
            }
            ifile >> word;
            if (word.length() == 3){
                ifile >> word;
                c = &b;
            }
            exp = atoi(word.c_str());
            if (exp < 0){
                error = "A negative exponent found in ";
                error += argv[1];
                throw error;
            }
            c->append(coef, exp, 1);
        }
        cout << "Polynomial 1:  ";  a.print();
        cout << "Polynomial 2:  ";  b.print();
        
        /*
            Do the math
        */ 
        Poly addition = a + b;
        Poly multiplication = a * b;
        Poly division = a / b;
        Poly mod = a % b;        
        
        /*
            Output results to file
        */ 
        ofstream ofile(argv[2]);
        
        addition.writeTo(ofile);
        ofile << "YYY" << endl;
        multiplication.writeTo(ofile);
        ofile << "ZZZ" << endl;
        division.writeTo(ofile);
        ofile << "RRR" << endl;
        mod.writeTo(ofile);
        
    }catch(string e){
        cout << "ERROR: " << e << endl;
        return 1;
    }
    cout << "Computed polynomial results successfully." << endl;
    return 0;
}

