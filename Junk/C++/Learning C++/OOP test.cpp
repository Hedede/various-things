#include <iostream>
#include <string.h>

    using namespace std;

    class employee
    {
    public:
       int assign_values(char *, long, float);
       void show_employee(void);
       int change_salary(float);
       long get_id(void);
    private:
       char name [64];
       long employee_id;
       float salary;
    };

    int employee::assign_values(char *emp_name, long emp_id, float emp_salary)
    {
       strcpy(name, emp_name);
       employee_id = emp_id;
        if (emp_salary < 50000.0)
        {
          salary = emp_salary;
          return(0); // Успешно
        }
        else 
        {
            return(-1); // Недопустимый оклад 
        }
    }

    void employee::show_employee(void)
    {
       cout << "SUZAZ: " << name << endl;
       cout << "NUMSU: " << employee_id << endl;
       cout << "OGLAD: " << salary << endl;
    }

    int employee::change_salary(float new_salary)

    {
       if (new_salary < 50000.0)
       {
          salary = new_salary;
          return(0); // Успешно 
        } else {
            return(-1); // Недопустимый оклад 
        }
    }

    long employee::get_id(void)
    {
       return(employee_id);
    }


    union vizooo
    {
        int yukh;
        long his_army;
        float opanki;
    } arrr;
    
  

int main()
{
    employee worker;
    if (worker.assign_values("Happy Jamsa", 101, 5000.0) == 0)
    {
        cout << "SUZAZEMU PRISVOENI " << endl;
        worker.show_employee();
        if (worker.change_salary(50000.00) == 0)
        {
            cout << "NOVYZ OZLAD ZOKHVACHEN" << endl;
            worker.show_employee();
        }
    } else {
        cout << "UPS OKLAD MORZHA" << endl;
    }
    arrr.yukh = 100500;
    cout << arrr.yukh << endl;
    arrr.his_army = 878454555;
    cout << arrr.yukh << endl;
    cout << arrr.his_army << endl;
    arrr.opanki = 0xFFFF;
       cout << arrr.opanki << endl; 
    cin.get();
    return 0;
}
