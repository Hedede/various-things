#include <iostream>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <math.h>

using namespace std;

extern __inline__ uint64_t rdtsc() {
   uint64_t x;
   __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
   return x;
}

inline double Root (double x, float base) {
    return x * pow(x, -base);
}

char vehnames[][20] = {"Vagina", "Dranduled", "Dran Ucozom duled"};

void findcar(char findname[20]) {
    int len = strlen(findname); 
    int cars = sizeof(vehnames);
    bool win = false;
    for(int i = 0; i <= cars; i++) {
        if(!strncmp(findname, vehnames[i], len)) {
            cout << "car id:" << i + 400 << ", car name: \"" << vehnames[i] << "\"\n";
            win = true;
        }
    }
    win ? cout << "> dovolen, kukufcheg?\n" : cout << "> nenashol mya tojego ucoza\n";
}

void findcar2(char findname[20]) {
    int length = strlen(findname);
    int tokens = 1;
	bool win = false;
	
	int i = 0;
	int b = 0; 
    while(i++ < 211) {
        while(findname[b] && (findname[b] == vehnames[i][b])) b++;
		if(b == length) {
            cout << "car id:" << i + 400 << ", car name: \"" << vehnames[i] << "\"\n";
            if(!win) win = true;
        }
		b = 0;
    }
    cout << (win ? "> dovolen, kukufcheg?\n" : "> nenashol mya tojego ucoza\n");
}

void printne() {
    int arrr = 16;
    arrr;
    std::cout << ~arrr << std::endl;
}

uint64_t GetCycleCount (void) {
    uint64_t CPS = rdtsc();
    Sleep (1000); 
    return rdtsc() -  CPS;
}

int unsigned time_s (uint64_t CycleCount) {
    return (int unsigned)(rdtsc() / CycleCount);  
}


int main() {
    int win;
    uint64_t start;
    teste:
    start = rdtsc();
    //findcar("Vag");
    //findcar("Dr");
    findcar2("Vag");
    findcar2("Dr");
    system("pause");
    cout << rdtsc() - start << endl;
    printne();
        
    uint64_t CPS = GetCycleCount ();
    
    testmarkerov:
    
    int unsigned asasa = time_s(CPS);
    
    
    cout << time_s(CPS) - asasa << endl;

    goto testmarkerov;
    /*uint64_t CPS;

    CPS = rdtsc();
    Sleep (1000); 
    CPS = rdtsc() -  CPS;
    
    cout << CPS << endl;
    
    //uint64_t start;

    start = rdtsc();
    cout << start << endl;

    int tiime = time(NULL);
    
    uint64_t qwewqwe;
    
    cin.get();
    
    qwewqwe = rdtsc();
    
    cout << qwewqwe << endl;
    
    uint64_t times = (qwewqwe - start) / CPS;
    
    cout << time(NULL) - tiime << endl;
    cout << times << endl;
    
    cout << Root(9, 2) << endl;
    win == 1 ? std::cout << "> win\n" : std::cout << "> fail\n";
    system("pause");
    cout << endl;
    goto teste;
    system("pause");*/
}
    
/*int wind;
double onuuulen = 0.;*/

/*void bigtest(bool bulb) {
    while(wind < 950000) {
        if(bulb) onuuulen = 3.145;
        wind++;
    }
    while(wind < 950000) {
        if(bulb) onuuulen = 3.145;
        wind++;
    }
    while(wind < 950000) {
        if(bulb) onuuulen = 3.145;
        wind++;
    }
    while(wind < 950000) {
        if(bulb) onuuulen = 3.145;
        wind++;
    }
    while(wind < 950000) {
        if(bulb) onuuulen = 3.145;
        wind++;
    }
}

void ripkotest() {
    while(wind < 950000) {
        onuuulen = 3.145;
        wind++;
    }
    while(wind < 950000) {
        onuuulen = 3.145;
        wind++;
    }
    while(wind < 950000) {
        onuuulen = 3.145;
        wind++;
    }
    while(wind < 950000) {
        onuuulen = 3.145;
        wind++;
    }
    while(wind < 950000) {
        onuuulen = 3.145;
        wind++;
    }
}*/

/*
    uint64_t start;
    //teste: 
    system("pause");
    cout << "Current: First pass //prochistko" << endl;
    start = rdtsc();
    //cout << start << endl;
    bigtest(true);
    cout << rdtsc() - start << endl;
    //cout << rdtsc() << endl;
    system("pause");
    cout << "Current: if(true)" << endl;
    start = rdtsc();
    //cout << start << endl;
    bigtest(true);
    cout << rdtsc() - start << endl;
    //cout << rdtsc() << endl;
    system("pause");
    cout << "Current: if(false)" << endl;
    start = rdtsc();
    //cout << start << endl;
    bigtest(false);
    cout << rdtsc() - start << endl;
    //cout << rdtsc() << endl;
    system("pause");
    cout << "Current: no if" << endl;
    start = rdtsc();
    //cout << start << endl;
    ripkotest();
    cout << rdtsc() - start << endl;
    //cout << rdtsc() << endl;
*/
/*
int findcar(char findname[20]) {
    int lenght;
    lenght = strlen(findname);
    int win = 0;
    for(int i = 0; i <= 211; i++) {
        int g = 1;
        for(int b = 0; g == 1 && b < lenght; b++) {
            if(findname[b] == vehnames[i][b]) {
                g = 1;
            } else {
                g = 0;
            }
        }
        if(g == 1) {
            omgochnulso = i + 400;
            cout << "car id:" << omgochnulso << ", car name: \"" << vehnames[i] << "\"\n";
            win = 1;
        }
    }
    if(win == 1) {
        cout << "> Поизг зовершон! доволен?\n";
    } else {
        cout << "> У меня нет мофынок с т кими интересными н зв ниями((\n";
    }
    return 1;
}
*/
