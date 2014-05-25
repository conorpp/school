
#include <iostream>
using namespace std;
class meow;

class woof{
	private:
		virtual void bow(){ cout << "bow"<< this->i <<endl; }
	public:
		woof(int i = 0){
			cout << "woof " << i << " constructed"<<endl;
			this->i = i;
			this->pi = i;
		}
		~woof(){
			cout << "woof " << this->i << " destructed" << endl;
		}

		virtual void bark(){cout << "bark"<<this->i<<endl;}	
		int pi,i;
};


class meow : public woof{
	public:
		meow(int g=0) {
			
			this->pi = 314;
			cout<< "meow constructed "<<this->pi<< endl; 
			woof::bark();

		}
		void bark(){cout<<"override bark "<< this->pi << endl;}
		~meow(){ cout << "meow deconstructed "<<endl; }
	private:
};

int main(){
	woof *w;
	meow a(1);
	w = &a;
	cout << "testing virtual "<< endl<<endl;
	w->bark();
	return 0;
}







