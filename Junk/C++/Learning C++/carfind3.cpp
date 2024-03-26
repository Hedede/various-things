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

char vehnames[][20] = {"Ucoz", "Dranduled", "Dran Ucozom duled", "Drun"};

/*void findcar(char findname[20]) {
    int len = strlen(findname); 
    int cars = sizeof(vehnames);
    for(int i = 0; i <= cars; i++) {
        if(!strncmp(findname, vehnames[i], len)) {
            cout << "car id:" << i + 400 << ", car name: \"" << vehnames[i] << "\"\n";
            cout << "> dovolen, kukufcheg?\n";
            return;
        }
    }
    cout << "> nenashol mya tojego ucoza\n";
}*/
void findcar(char findname[20]) {
    int len = strlen(findname); 
    int cars = sizeof(vehnames);
    int tokens = 0;
    for(int i = 0; i <= cars; ++i) {
        if(!strncmp(findname, vehnames[i], len)) {
            cout << "car id:" << i + 400 << ", car name: \"" << vehnames[i] << '"' << endl;
            ++tokens;
        }
    }
    if(!tokens) {
        cout << "> nenashol mya tojego ucoza" << endl;
    };
}

void printne() {
    int arrr = 16;
    arrr;
    std::cout << ~arrr << std::endl;
}

int main() {
    int win;
    uint64_t start;
    teste:
    start = rdtsc();
    findcar("Ucoz");
    findcar("Dr");
    findcar("azA");
    cout << rdtsc() - start << endl;
    printne();
    //cout << Root(9, 2) << endl;
    win == 1 ? std::cout << "> win\n" : std::cout << "> fail\n";
    system("pause");
    cout << endl;
    FILE * pFile;
    char araaaat[] = { 0xAE , 0x99 , 0xB2 };
    pFile = fopen ( "E:\\Projects\\BepeQuest\\Test\\paeben.pbn2" , "wb" );
    if(fwrite(araaaat , 1 , sizeof(araaaat), pFile)) {
        cout << "> win\n";
    };
    fclose (pFile);
    goto teste;

    system("pause");
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
