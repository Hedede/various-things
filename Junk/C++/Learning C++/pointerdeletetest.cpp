#include <iostream>

using namespace std;


int main()
{
    int *p=new int;
    system("echo יאאא, ץףוחמח!");
    *p=10;
    cout << "ADDRESS OF P is " << p << endl;
    delete p;
    int * x= new int(3);
    cout << "ADDRESS OF X=" << *x << " is " << x << endl;
    //*p = 10; 
    for(int i = 0; i < 80; i++, x++);
    cout << "ADDRESS OF X : " << x << endl;
    cout << "VALUE OF X : " << *x << endl;
    int * w= new int(3);
    cout << "ADDRESS OF W=" << *w << " is " << w << endl;
        system("PAUSE");
        cin.get();
}
