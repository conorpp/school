/*
    Conor Patrick, HW5
    04/22/2014
    mergesort.cpp
        Client code for the merge sort algorithm
        Usage:
            ./mergesort <input-file> <output-file>
*/
// #include "mergesort.h"
#include "huffman.h"
#include <sstream>
#include <vector>

int main(int argc, char *argv[]){
    try{
        
        string err;
        if (argc < 3){
          err = "You must provide at least two arguments: \n ./mergesort <input-file> <output-file> ";
          throw err;
        }
        
        /*  Open the file  */
        
        ifstream ifile(argv[1]);
        if (!ifile.is_open()){
          err = "The file ";
          err += argv[1];
          err += " does not exist.";
          throw err;
        }
        
        vector <item> items;
        string buff;
        char letter;
        int freq;
        item *a;
        
        while (ifile >> buff){

            letter = *buff.c_str();
            if (ifile >> buff){
                stringstream convert (buff);
                convert >> freq;
            }else {
                err = "Expected number after \"";
                err += letter;
                err += "\" was read.";
                throw err;
            }
            if (! letter || (!freq and freq != 0))
                throw string("invalid input given");
                
            a = new item(letter, freq);
            
            if (a == NULL){
                err = "Memory allocation returned NULL in int main().";
                throw err;
            }

            items.push_back( *a );
            
        }

        
        /*   output for each teller to file   */
        ofstream ofile(argv[2]);
        if (!ofile.is_open()){
            throw string("cannot create output file: ")+argv[2];
        }
        /*   create objects using pointer to object data   */
        mergeSort sort(items.data(), items.size());
        sort.write(ofile);

    }catch(string _err){
        cout << "ERROR: " << _err << endl;
        return 1;
    }
    cout << "Merge sort completed successfully. " << endl;
    return 0;
}