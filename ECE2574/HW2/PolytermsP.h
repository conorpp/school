/*
    Conor Patrick
    03/04/2014
    
    Filename: Poly.h
    Polynomial computation class.
*/

#include <fstream>

typedef float CoefficientItem;

class Poly{
        
    public:
        // Constructor, copy constructor, destructor
        Poly();
        Poly(const Poly& rhs);
        ~Poly();
        
        // Add or delete from Polynomial linked list.
        void append(CoefficientItem coef,  int exp, int simple);
        void del( int exp );
        
        // Helper functions
        void print();
        void simplify();
        void order();
        void writeTo(std::ofstream& file);
        
        // Math functions for adding, multiplying, and dividing
        Poly& operator=(const Poly& rightSide);
        
        Poly& operator+=(const Poly& rhs);
        const Poly operator+(const Poly& rhs) const;
        
        Poly& operator*=(const Poly& rhs);
        const Poly operator*(const Poly& rhs) const;
        
        Poly divideBy(const Poly& rhs, int remainder);
        Poly& operator/=(const Poly& rhs);
        const Poly operator/(Poly& rhs);
        const Poly operator%(Poly& rhs);
        
        
        // Linked list node
        struct Node{
            CoefficientItem coefficient;
            int exponent;
            Node* next;
        };

    private:
        // Start of linked list
        Node *head;
};

