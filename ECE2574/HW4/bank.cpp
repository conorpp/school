#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include "simulate.h"

using namespace std;

int main(int argc, char *argv[]){
    try{
        
        string err;
        if (argc < 3){
          err = "You must provide at least two arguments: \n ./bank <input-file> <output-file> [<number-of-tellers>]";
          throw err;
        }
        
        /*  Open the file  */
        
        string word;
        ifstream ifile(argv[1]);
        if (!ifile.is_open()){
          err = "The file ";
          err += argv[1];
          err += " does not exist.";
          throw err;
        }
        
        /*  Parse file for dynamic amount of tellers */
        
        int tellers = 1;
        if (argc >= 4){
            if (atoi(argv[3]))
                tellers = atoi(argv[3]);
        }
        
        cout << "Starting simulation for " << tellers;
        if (tellers > 1) cout << " tellers...";
        else cout << " teller...";
        cout << endl;
        
        EventList eventBuckets[tellers];
        int i = tellers;
        while (i--){
            eventBuckets[i] = EventList();
        }
        i = 0;
        string time, transaction;
        Arrival *a;
        
        while (ifile >> word){
            
            time = word;
            if (ifile >> word){
                transaction = word;
            }else {
                err = "Expected number after \"";
                err += time;
                err += "\" was read.";
                throw err;
            }
            a = new Arrival(time, transaction);
            
            if (a == NULL){
                err = "Memory allocation returned NULL.";
                throw err;
            }

            eventBuckets[i].line.push(*a);
            
            i = (i >= (tellers - 1)) ? 0 : i + 1;
            
        }
        
        /*   output for each teller to file   */
        ofstream ofile(argv[2]);
        for (i = 0; i < tellers; i++){
            eventBuckets[i].findEvents();
            ofile << "------Teller line " << (i+1) << "------" << endl << endl; 
            eventBuckets[i].printEvents(ofile);
            eventBuckets[i].printStats(ofile);
            ofile << endl;
        }
    
    }catch(string _err){
        cout << "ERROR: " << _err << endl;
        return 1;
    }
    cout << "Simulation completed successfully. " << endl;
    return 0;
}
