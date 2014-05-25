
#include <iostream>

class queue{
    
    public:
        queue(char a='\0'){
            this->data = NULL;
            if (a) this->add(a);
        }
        ~queue(){
            if (this->data != NULL) delete[] data;
        }
        queue & add(char a){
            int l = this->length() + 1;
            char *tmp = new char[l--];
            tmp[l] = a;
            while(l--){
                tmp[l] = this->data[l];
            }
            delete[] this->data;
            this->data = tmp;
            return (*this);
        }
        int length(){
            if (this->data == NULL) return 0;
            int l = 0;
            while(*(this->data+l)){
                l++;
            }
            return l;
        }
        char *slice(int l = 0){
            if (l > this->length() - 1) return (this->data+this->length()-1);
            return (this->data+l);  
        };
        char shift(){
            if (this->data == NULL) return '\0';
            char d = this->data[0];
            int l = this->length();
            char *tmp;

            if (l){
                tmp = new char[ (--l) ];
                while(l--){
                    tmp[l] = this->data[l+1];
                }
            }
            else tmp = NULL;
            
            delete[] this->data;
            this->data = tmp;
            
            return d;
        }
        
    private:

        char *data;
    
};

int main(){
    
    queue events('a');
    events.add('a').add('a').add('a').add('d').add('d').add('d').add('a').add('a').add('a').add('a').add('d').add('d').add('d').add('a');
            
    queue a;

    char c = events.shift();
    
    while(c){
        if (c=='a')
            a.add('x');
        else if (c=='d')
            a.shift();
        c = events.shift();

    }
    
    char * p = a.slice();
    while(*p){
        std::cout << *p << std::endl;
        p++;
    }
    
    
    return 0;
    
}