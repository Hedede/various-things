#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    //declare variables
    const int MAXITEMS = 4;
    string lastItem;
    string inventory[MAXITEMS] = {"Sword", "Armor", "Shield"};
    string replaceItem;
    string addItem;
     // print what is in inventory to start with
     cerr << "We start with\n" << endl;
     
     for (int i = 0; i < MAXITEMS; ++i)
     {                 
         lastItem = inventory[i];
         cerr << inventory[i] << endl << endl;                  
     }
     
     
     cerr << "Replace Item one in the inventory. " << endl << endl;
     getline(cin,replaceItem);
     inventory[0] = replaceItem;
     cerr << endl << endl;
     cerr << "We now have\n" << endl << endl;
           
     for (int i = 0; i < MAXITEMS; ++i)
     {
         lastItem = inventory[i];
         cerr << inventory[i] << endl << endl;                  
     }
     
        cerr << "Add an Item to the inventory. " << endl << endl;
        getline(cin, addItem);
        inventory[3] = addItem;
        cerr << endl << endl;
        cerr << "We now have\n" << endl << endl;

        for (int i = 0; i < MAXITEMS; ++i)
        {             
          
            lastItem = inventory[i];
            cerr << inventory[i] << endl << endl;            
        }
             cerr << "error message" << endl << endl; 
        
    system("pause");
    return EXIT_SUCCESS;
}
