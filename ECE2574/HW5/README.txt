
Source files:
    huffman.h
    huffman_source.cpp
    mergesort.h
    mergesort_source.cpp
    
client code / programs:
    mergesort.cpp
    huffman.cpp
    
Extra credit:
    I did the extra credit.  All binary paths are "zero optimized"
    
You must compile all of the source files and only one of the
client programs to run.  Compiling both client programs will fail
because there would be two int main() functions.

To compile and run mergesort program:
    g++ huffman_source.cpp mergesort_source.cpp mergesort.cpp -o mergesort
    ./mergesort <input-file> <output-file>

To compile and run huffman program:
    g++ huffman_source.cpp mergesort_source.cpp huffman.cpp -o huffman
    ./huffman <input-file> <output-file>
    
    