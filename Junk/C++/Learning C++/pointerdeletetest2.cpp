#include <iostream>

using namespace std;


int main()
{
        int *p=new int;
        *p=10;
        delete p;
        cout << *p << endl;
        *p=     60;
        cout << *p << endl;
        system("PAUSE");
        cin.get();
}
