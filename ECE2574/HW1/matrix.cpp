/*
 *  Conor Patrick
 *  
    Implementation File
*/

#include <iostream>
#include "matrix.h"
using namespace std;

/* constuctor */
Matrix::Matrix(int row_size, int column_size){
    this->_init(row_size, column_size);
}

/* copy constructor */
Matrix::Matrix(const Matrix& orig){
    this->_init(orig.rows, orig.columns);
}

/* destructor */
Matrix::~Matrix(){
    if (this->data && this->rows && this->columns)
        delete[] this->data;
};

/* for initializing and reusing code for constructor and copy constructor. */
void Matrix::_init(int row_size, int column_size){
    try{
        if (row_size < 1 || column_size < 1) throw "incorrect matrix dimensions";
        
        rows = row_size;
        columns = column_size;
        int length = row_size * column_size;
        this->data = new MatrixItem [length];
        if (this->data != NULL){    //set default values if not null
            for(int r = 0; r<row_size; r++)
                for(int c = 0; c<column_size; c++)
                    this->setElement(r, c, r - c);
                    
        }else throw "memory error. no allocated space.";
    }catch(char const *err){
        cout << "MATRIX ERROR: " <<err << endl;
    }
}

/* set element at (row, column) position */
void Matrix::setElement(int row, int column, MatrixItem value){
    try{
        if (this->data == NULL) throw "memory error. no allocated space in data for matrix.";
        
        this->data[row*this->columns + column] = value;
    
    
    }catch(char const *err){
        cout << "MATRIX ERROR: " <<err << endl;
    }
};

/* returns element at (row, column) position */
MatrixItem Matrix::getElement(int row, int column) const{
    try{
        if (row > this->rows || column > this->columns)
            throw "out of boundaries for getting element";
        if (this->data == NULL)
            throw "memory error. no allocated space in data for matrix.";
        
        return this->data[row*this->columns + column];
    
    }catch(char const *err){
        cout << "MATRIX ERROR: " <<err << endl;
        return 0;
    }
};

/* overload << for printing */
ostream& operator<<(ostream& out, Matrix& m){
    for (int r = 0; r< m.rows; r++){
        for (int c = 0; c< m.columns; c++){
            MatrixItem ele = m.getElement(r, c);
            if (ele >= 0) out << " ";       //padding
            out << " " << ele;
        }
        out << endl;
    }
    return out;
};

/* overload for copying one matrix over another */
Matrix& Matrix::operator=(const Matrix& orig){
    Matrix wtf = *this;
    Matrix wtf2 = orig;
    cout << "the matrix1 "<<endl << wtf << endl;
    cout << "the matrix2 "<<endl << wtf2 << endl;
    int length = orig.rows * orig.columns;
    this->rows = orig.rows;
    this->columns = orig.columns;
    if (this->data != NULL) delete[] this->data;
    this->data = new MatrixItem[length];
    while(length--){
        this->data[length] = orig.data[length];
    }

    return *this;
};

/* overload + for adding matrixes */
Matrix Matrix::operator+(const Matrix& orig) const{
    try{
        
        if (this->rows != orig.rows || this->columns != orig.columns){
            throw "You\'re adding together two matrixes of different dimensions";
        }
        Matrix temp = *this;
        int length = temp.columns*temp.rows;
        while(length--)
            temp.data[length] += orig.data[length];
        cout << "the temp" << endl << temp << endl;

        return temp;
    
    
    }catch(char const *err){
        cout << "MATRIX ERROR: " <<err << endl;
        return orig;
    }
};

/* overload += for adding matrixes */
Matrix& Matrix::operator+=(const Matrix& orig) {
    try{
        if (this->rows != orig.rows || this->columns != orig.columns){
            throw "You\re adding together two matrixes of different dimensions";
        }
        int length = this->columns*this->rows;
        while(length--)
            this->data[length] += orig.data[length];
    }catch(char const *err){
        cout << "MATRIX ERROR: " <<err << endl;
    }
    return *this;
};

/*   overload * for matrix multiplying   */
Matrix Matrix::operator*(const Matrix& orig)const{
    
    try{
        if (this->columns != orig.rows){
            throw "Cannot multiply matrixes, illegal dimensions.";
        }
        MatrixItem element = 0;
        Matrix temp (this->rows , orig.columns);
        temp.columns = orig.columns;
        temp.rows = this->rows;
        // Go through each column and row and add them
        for (int x = 0; x<temp.rows; x++){
            for (int y = 0; y<orig.columns; y++){
                for(int iter = 0; iter<orig.rows; iter++){
                    element+= (
                               this->getElement(x, iter) *
                               orig.getElement(iter, y)
                            );
                }
                temp.setElement(x, y, element);
                element = 0;
            }
        }
        return temp;
    }catch(char const *err){
        cout << "MATRIX ERROR: " <<err << endl;
        return orig;
    }
};

/*   overload *= for matrix multiplying   */
Matrix& Matrix::operator*=(const Matrix& orig){
    try{
        if (this->columns != orig.rows){
            throw "Wrong dimensions for multiplying";
        }
        MatrixItem element = 0;
        this->columns = orig.columns;
        this->rows = this->rows;
        for (int x = 0; x<this->rows; x++){
            for (int y = 0; y<orig.columns; y++){
                for(int iter = 0; iter<orig.rows; iter++){
                    
                    element+= (
                               this->getElement(x, iter) *
                               orig.getElement(iter, y)
                            );
                }
                this->setElement(x, y, element);
                element = 0;
            }
        }
    }catch(char const *err){
        cout << "MATRIX ERROR: " <<err << endl;
    }
};

//  overload == and != to test for matrix equality.
bool Matrix::operator==(const Matrix& orig)const{
    int l1 = this->columns * this->rows;
    int l2 = orig.columns * orig.rows;
    if (l1 != l2) return false;
    while(l1--){
        if (this->data[l1] != orig.data[l1]) return false;
    }
    return true;
}

bool Matrix::operator!=(const Matrix& orig)const{
    
    return !(*this == orig);
}

int main(){
    Matrix a(2,2);
    Matrix b(2,2);
    Matrix c(2,2);
    c=(a+b);
    cout <<"c" <<endl<<c << endl;
    cout <<"a" <<endl<<a << endl;
    cout <<"b" <<endl<<b << endl;
}

