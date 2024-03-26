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

/* Канализация */
void primitie()
{
    found:
    CharToOem("Ога! Кажется это унитаз!",Buffer);
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
         CharToOem("***** Извини чуваг, бето, бонусов нема *****",Buffer);
         cout << Buffer << endl;
    }
    CharToOem("1. Примытсо",Buffer);
    cout << Buffer << endl;
    if(nUnitazov < 100)
    { 
        CharToOem("2. Найти другой унитаз",Buffer);
        cout << Buffer << endl;
    }
    input:
    cout << ">"; 
    cin >> in;
    if(in == "1"){
        CharToOem("Уууу я примыылось!",Buffer);
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
    CharToOem("Ты наступил на канализационный люк и провалилсо в канализацию!",Buffer);
    cout << Buffer << endl;
    CharToOem("1. Пойти искать хуйло",Buffer);
    cout << Buffer << endl;
    CharToOem("2. Попытаться вылезти",Buffer);
    cout << Buffer << endl;
    CharToOem("3. Поискать место для примытия",Buffer);
    cout << Buffer << endl;    
    input:
    cout << ">"; 
    cin >> in;
    if(in == "1"){
        CharToOem("Ты задохнулся пока искал хуйло...",Buffer);
        cout << Buffer << endl;
        gameover();
    } else if(in == "2") {
        cout << name;
        CharToOem(" пытается вылезти...",Buffer);
        cout << Buffer << endl;
        CharToOem("Упс, ",Buffer);
        cout << Buffer;
        cout << name;
        CharToOem(" наёбнулся!",Buffer);
        cout << Buffer << endl;
        gameover();
    } else if(in == "3") {
        primitie();
    } else {
        goto input;
    }
}

/* УкУрок */
void ulico()
{
    CharToOem("УкУрок убежал",Buffer);
    cout << Buffer << endl;
    CharToOem("1. Пойти искать его",Buffer);
    cout << Buffer << endl;
    CharToOem("2. Остатсо на месте",Buffer);
    cout << Buffer << endl;
    CharToOem("3. Пойти куда нибудь",Buffer);
    cout << Buffer << endl; 
    input:
    cout << ">"; 
    cin >> c;
    switch(c)
    {
        case '1' : gameover();
        break;
        case '2' : CharToOem("Упс... \n",Buffer);
        cout << Buffer; 
        cout << name;
        CharToOem(" спизжен by COTM",Buffer);
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
    CharToOem("-Аааа гдеее моооой друуууг рооооозооовыыый слоооооонииик?!",Buffer);
    cout << Buffer << endl;
    CharToOem("1. -Пошли вместе поищем",Buffer);
    cout << Buffer << endl;
    CharToOem("2. -Ой, мне пора идти *уходит*",Buffer);
    cout << Buffer << endl;
    CharToOem("3. -Таааааам!",Buffer);
    cout << Buffer << endl;       
    input:
    cout << ">"; 
    cin >> c;
    switch(c)
    {
        case '1' : cout << "Uexal nahui..." << endl;
        gameover();
        break;
        case '2' : /*CharToOem("-Пооодооождииии! Яяяя хооотеееел поооознааакооомииить теееебяяя с моооииим друуугоом рооооозооовыыым слоооонииикооом!",Buffer);
        cout << Buffer << endl;*/
        canal();
        break;
        case '3' : CharToOem("Гдееее?! Оооой саааамтрии... Ааааа поомоогиитее крааасныыыеее! *убегает*",Buffer);
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

/* Начало */
void quest()
{
    system("cls");
    CharToOem("Итаг, ",Buffer);
    cout << Buffer;
    cout << name;
    CharToOem(", Поехале!",Buffer);
    cout << Buffer << endl;
    CharToOem("1. Нахуй!",Buffer);
    cout << Buffer << endl;
    CharToOem("2. Пойти пешком",Buffer);
    cout << Buffer << endl; 
    CharToOem("3. Остатсо на месте",Buffer);
    cout << Buffer << endl; 
    input:
    cout << ">";
    cin >> c;
    switch(c)
    {
        case '1' : CharToOem("Уехал нахуй...",Buffer);
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
    CharToOem("1. Попробовать еще раз",Buffer);
    cout << Buffer << endl; 
    CharToOem("Или введите что либо другое для выхода.",Buffer);
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
    CharToOem("Превед! Эта БеПе-Quest!",Buffer);
    cout << Buffer << endl;
    CharToOem("Каг тибя зовут, далбайобчег?",Buffer);
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

