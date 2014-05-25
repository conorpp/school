/* 

    Conor Patrick
    Homework 3
    searchmap.cpp - client code
    
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "mapclass.h"

using namespace std;

void checkCity(string w){
  if (w.length() != 1){
    string error;
    error = "City: \"";
    error += w;
    error += "\" is not a single character.";
    throw error;
  }
}

int main(int argc, char* argv[]) {
  try{
    
    string error;
    
    /*
      Check Args
    */
    if (argc < 3){
      error = "You must provide two arguments: \n ./searchmap <input-file> <output-file>\n";
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
    
    Map map;
    char origin;
    char oCity = '\0';
    char dCity = '\0';
    Price price = 0;
    int offset = 0;
    int first = 0;
    
    /*
      Grab origin city
    */
    getline(ifile, word);
    if (word.length() != 2){
      throw string("Invalid origin city");
    }else{
      origin = *word.c_str();
    }
    
    /*
      Get the rest of the file
    */
    while(ifile >> word){

      switch(offset){
        case 0:           //first city
          checkCity(word);
          oCity = *word.c_str();
        break;
        case 1:           //second city
          checkCity(word);
          dCity = *word.c_str();
        break;
        case 2:           // price
          price = atoi(word.c_str());
          if (!price) throw string("Invalid price specified.");
        break;
      }
      
      offset = (offset >= 2) ? 0 : (offset+1);

      if (oCity && dCity && price){
        // Add to map
        map.addPoint(oCity, dCity, price);  
        oCity = dCity = '\0';
        price = 0;
      }
    }
    
    map.setOrigin(origin);
    map.Mapout();
    
    /*
      Prepare for output
    */
    ofstream ofile(argv[2]);
    map.printMap(ofile);
    
    
     
  }catch(string e){
    cout << "Error: "<< e<< endl;
    return 1;
  }
  
  cout << "Successfully completed map." << endl;
  
  return 0;
}
