#include <iostream>
//#include <stdlib.h>
//#include <stdio.h>
#include <windows.h>

using namespace std;

void gameover (void);
//void quest ();
//void canal ();

char Buffer[128];
int nUnitazov;

char c, name[32];

string in;

/* ����������� */
void primitie()
{
    found:
    CharToOem("���! ������� ��� ������!",Buffer);
    cout << Buffer << endl;
    if(nUnitazov == 0) {
        nUnitazov = 1;
        cout << "***** 1 of 100 unitazov found *****" << endl;
    }
    else if(nUnitazov > 0 && nUnitazov < 100) {
         cout << "***** " << nUnitazov << " of 100 unitazov found *****" << endl;
    }
    else if(nUnitazov == 100) {
         cout << "***** 100 of 100 unitazov found! *****" << endl;
         CharToOem("***** ������ �����, ����, ������� ���� *****",Buffer);
         cout << Buffer << endl;
    }
    CharToOem("1. ��������",Buffer);
    cout << Buffer << endl;
    if(nUnitazov < 100)
    { 
        CharToOem("2. ����� ������ ������",Buffer);
        cout << Buffer << endl;
    }
    input:
    cout << ">"; 
    cin >> in;
    if(in == "1"){
        CharToOem("���� � ����������!",Buffer);
        cout << Buffer << endl;
        gameover();
    } else if((in == "2") && (nUnitazov < 100)) {
        nUnitazov++;
        goto found;
    } else {
        goto input;
    }
}

void canal()
{
    CharToOem("�� �������� �� ��������������� ��� � ���������� � �����������!",Buffer);
    cout << Buffer << endl;
    CharToOem("1. ����� ������ �����",Buffer);
    cout << Buffer << endl;
    CharToOem("2. ���������� �������",Buffer);
    cout << Buffer << endl;
    CharToOem("3. �������� ����� ��� ��������",Buffer);
    cout << Buffer << endl;    
    input:
    cout << ">"; 
    cin >> in;
    if(in == "1"){
        CharToOem("�� ���������� ���� ����� �����...",Buffer);
        cout << Buffer << endl;
        gameover();
    } else if(in == "2") {
        cout << name;
        CharToOem(" �������� �������...",Buffer);
        cout << Buffer << endl;
        CharToOem("���, ",Buffer);
        cout << Buffer;
        cout << name;
        CharToOem(" ��������!",Buffer);
        cout << Buffer << endl;
        gameover();
    } else if(in == "3") {
        primitie();
    } else {
        goto input;
    }
}

/* ������ */
void ulico()
{
    CharToOem("������ ������",Buffer);
    cout << Buffer << endl;
    CharToOem("1. ����� ������ ���",Buffer);
    cout << Buffer << endl;
    CharToOem("2. ������� �� �����",Buffer);
    cout << Buffer << endl;
    CharToOem("3. ����� ���� ������",Buffer);
    cout << Buffer << endl; 
    input:
    cout << ">"; 
    cin >> c;
    switch(c)
    {
        case '1' : gameover();
        break;
        case '2' : CharToOem("���... \n",Buffer);
        cout << Buffer; 
        cout << name;
        CharToOem(" ������� by COTM",Buffer);
        cout << Buffer << endl;
        gameover();
        break;
        case '3' : canal();
        break;
        default : goto input;
        break;
    }
}

void ukurok()
{
    CharToOem("-���� ����� ������ ������� ��������������� �������������?!",Buffer);
    cout << Buffer << endl;
    CharToOem("1. -����� ������ ������",Buffer);
    cout << Buffer << endl;
    CharToOem("2. -��, ��� ���� ���� *������*",Buffer);
    cout << Buffer << endl;
    CharToOem("3. -��������!",Buffer);
    cout << Buffer << endl;       
    input:
    cout << ">"; 
    cin >> c;
    switch(c)
    {
        case '1' : cout << "Uexal nahui..." << endl;
        gameover();
        break;
        case '2' : /*CharToOem("-��������������! ���� ���������� �������������������� ��������� � �������� ��������� ��������������� ���������������!",Buffer);
        cout << Buffer << endl;*/
        canal();
        break;
        case '3' : CharToOem("������?! ����� ����������... ����� ������������ �������������! *�������*",Buffer);
        cout << Buffer << endl;
        ulico();
        break;
        default : goto input;
        break;
    }
}

char c2;
char c3;
char lol[128];

/* ������ */
void quest()
{
    system("cls");
    CharToOem("����, ",Buffer);
    cout << Buffer;
    cout << name;
    CharToOem(", �������!",Buffer);
    cout << Buffer << endl;
    CharToOem("1. �����!",Buffer);
    cout << Buffer << endl;
    CharToOem("2. ����� ������",Buffer);
    cout << Buffer << endl; 
    CharToOem("3. ������� �� �����",Buffer);
    cout << Buffer << endl; 
    input:
    cout << ">";
    cin >> c;
    switch(c)
    {
        case '1' : CharToOem("����� �����...",Buffer);
        cout << Buffer << endl;
        gameover();
        break;
        case '2' : canal();
        break;
        case '3' : ukurok();
        break;
        default : goto input;
        break;
    }
}


void gameover(void)
{
    cout << "Game Over, " << name << "!" << endl; 
    CharToOem("1. ����������� ��� ���",Buffer);
    cout << Buffer << endl; 
    CharToOem("��� ������� ��� ���� ������ ��� ������.",Buffer);
    cout << Buffer << endl; 
    cout << ">"; 
    cin >> c;
    switch(c)
    {
        case '1' : quest();
        break;
        default : exit(0);
        break;
    }
}

int main (int argc, char *argv[])
{
    //cout << '\a';
    CharToOem("������! ��� ����-Quest!",Buffer);
    cout << Buffer << endl;
    CharToOem("��� ���� �����, �����������?",Buffer);
    cout << Buffer << endl; 
    cout << ">"; 
    DWORD ProcId;
    HWND hWnd = FindWindow(0, "Google Talk");
    GetWindowThreadProcessId(hWnd, &ProcId);
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcId);
     cout << ProcId << endl; 
    cin.getline(name, sizeof(name)); 
    quest();
    return 0;
}

