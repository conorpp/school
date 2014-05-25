// Conor Patrick.  Extra Credit
#include <iostream>
#include <string>
#include <stdlib.h>


using namespace std;
#define MAXSIZE 2000
#define MAX_NUM 99999

class mergeSort{

	public:
	mergeSort(){ size=0; align = 0;}
	void echo(){		// print it out
		cout << "array: ";
		int i = 0;
		while (i<(size-align)){
			cout << data[(i+align)];
			if (++i< size) cout << ", ";
		}
		cout << endl;
	}
	void add(int a){	// add an int
		data[size] = a;
		size++;
	}
	/*
		Make the array length a power of 2 to fit sorting frame.
	*/
	void _align(){
		int pow =size;
		int min = MAX_NUM;
		if (pow == 2) return;
		for (int i=0; i<size; i++){
			if ( data[i] < min ) min = data[i];
		}
		while(pow != 2){
			pow = pow/2;
			if (!pow){
				this->add(min);
				this->align++;
				pow = size;
			}
			
		}
	}

	/*
		Merge sort the data
	*/

	void sort(){
		this->_align();
		int sizeOffset = 1;
		while(sizeOffset < size){
			for (int i=0; (i+1)*sizeOffset < size; i=i+2){
				if (data[sizeOffset*i] > data[sizeOffset*(i+1)]){		// swap memory locations
					

					cout << "swapping "; 
					int tmp[sizeOffset];
					copy(tmp, data, sizeOffset, 0, sizeOffset*i );		// make tmp copy of first sub array
					copy(data, data, sizeOffset, sizeOffset*i, sizeOffset*(i+1)); // overwrite first sub array with next sub array
					copy(data, tmp, sizeOffset, sizeOffset*(i+1), 0);	// overwrite next sub arrary with tmp
					this->echo();
				}
			}
			sizeOffset *= 2; 		// increase sub array size
			cout << "sizeoffset: "<< sizeOffset << endl;
		}
		
		cout << "done "<<endl;
		
	}
	/*
		Copy one array to another with respective offsets
	*/
	void copy(int tmp[], int dat[], int size, int offset1, int offset2){		
		while (size--)
			tmp[size+offset1] = dat[size+offset2];
	}
	int data [MAXSIZE];
	int size, align;
		
};


int main(){
	
	cout << "please enter integers you\'d like to be merge sorted one at a time." << endl;
	cout << "terminate the input and start sorting by entering a \'merge\'" << endl;
	
	int i=0;
	string s;
        
	mergeSort bucket;
	while (1){
		
		cin >> s;
		if (s == "merge")break;
		i = atoi(s.c_str());
		
		bucket.add(i);
		bucket.echo();
	}
	bucket.sort();
	bucket.echo();

	return 0;
}
