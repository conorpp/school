////////////////////////////////////////////////////////
// ECE 2574, Homework 1, Professor Wang
// Conor Patrick
//
// File name: matrix.h
// Description: Header file for the ADT Matrix
// Date: 2/8/2014
//
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

typedef double MatrixItem;

class Matrix{
    private:
        int rows;
        int columns;
        MatrixItem *data;

        friend ostream& operator<<(ostream& out, Matrix& m);
        void _init(int row_size, int column_size);
        
    public:
        Matrix(int row_size, int column_size);
        Matrix(const Matrix& orig);
        ~Matrix();
        
        void setElement(int row, int column, MatrixItem value);
        MatrixItem getElement(int row, int column) const;
        
        Matrix& operator=(const Matrix& orig);
        Matrix operator+(const Matrix& orig) const;
        Matrix operator*(const Matrix& orig) const;
        
        bool operator==(const Matrix& orig) const;
        bool operator!=(const Matrix& orig) const;
        
        Matrix& operator*=(const Matrix& orig);
        Matrix& operator+=(const Matrix& orig);


}; // End Matrix class
