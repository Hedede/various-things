#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;

int makerandom(int digits, int seed = 0) {
    int randum = 0;
    int z;
    for(int i = 1; i < digits; i++) {    
        srand(unsigned((time(0) - (int)pow(10.,i) + i * z) * i + seed));
        z = rand();
        randum += (z % 10) * (int)pow(10.,i) + 1;
    }
    return randum;
}

int makerandom2(int min, int max) {
    int randum = 0;
    int z;
    for(int i = 1; i < 5; i++) {    
        srand(unsigned((time(0) - (int)pow(10.,i) + i * z) * i + 5));
        z = rand();
    }    
    z = rand();    
    randum += (z % (max - min + 1)) + min;
    return randum;
}

int main() {
    input:   
    system("cls");
    //int vararandum = (z % 10) + (z % 10)*10 + (z % 10)*100 + (z % 10)*1000 + (z % 10)*1000 
    //+ (z % 10)*10000 + (z % 10)*100000 + (z % 10)*1000000;
   cout << " = " << makerandom2(1000, 1999) << endl;
   cout << " = " << makerandom(4, 1999) << endl;
    system("pause");
    goto input;
}

