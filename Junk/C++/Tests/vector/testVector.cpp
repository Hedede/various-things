#include<iostream>
#include <string>
#include "vector.h"

int main( int argc, char **argv ) {
    std::string s = argv[1];
    // 1- Test the default constructor
    if (s == "1")
    {
        vector<float> v1;
        vector<int> v2;
        vector<std::string> v3;
    }
    if (s == "2")
    {
        vector<float> v1(1);
        v1.data[0] = 10.0f;
        INFO(v1);
    }
    if (s == "3")
    {
        vector<int> v1(5);
        for (int i = 1; i < v1.getSize(); i++)
            v1.data[i] = i;
        INFO(v1);
    }
    if (s == "4")
    {
        float a[] = {1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f};
        vector<float> v1(a, a+8); // Using an overloaded constructor
        INFO(v1);
    }
    if (s == "5")
    {
        vector<std::string> v1(2);
        v1.data[0] = "one";
        v1[1] = "two";
        INFO(v1);
    }
    if (s == "6")
    {
        vector<float> v1(2);
        v1.data[0] = 1;
        v1.data[0] = 1;
        INFO(v1);
    }
    if (s == "7")
    {
        float a[] = {1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f};
        vector<float> v1; // Using an overloaded constructor
        v1.assign(a,a+8);
        INFO(v1);
    }
    if (s == "8")
    {
        vector<float> a(8);
        vector<float> b(8);
        for (size_t i = 0; i< a.getSize(); i++)
        {
            a[i] = i+1;
            b[i] = i+1;
        }
        vector<float> v1 = a.add(b);
        INFO(v1);
    }
    if (s == "9")
    {
        vector<float> a(8);
        vector<float> b(8);
        for (size_t i = 0; i< a.getSize(); i++)
        {
            a[i] = i+1;
            b[i] = i+1;
        }
        vector<float> v1 = a + b;
        INFO(v1);
    }
    if (s == "10")
    {
        vector<float> b(8);
        for (size_t i = 0; i< b.getSize(); i++)
        {
            b.data[i] = i+1;
        }
        b.resize(10);
        b.data[8] = 9; b.data[9] = 10; 
        INFO(b);
    }
    if (s == "11")
    {
        vector<float> b(8);
        b.data[0] = 11.3f;
        b.data[1] = 12.3f;
        b.data[2] = 21.3f;
        b.data[3] = 31.3f;
        b.data[4] = 1.3f;
        b.data[5] = 71.3f;
        b.data[6] = 18.3f;
        b.data[7] = 11.4f;
        b.BubbleSort();
        INFO(b);
    }
    if (s == "12")
    {
        vector<float> b(8);
        b.data[0] = 11.3f;
        b.data[1] = 12.3f;
        b.data[2] = 21.3f;
        b.data[3] = 31.3f;
        b.data[4] = 1.3f;
        b.data[5] = 71.3f;
        b.data[6] = 18.3f;
        b.data[7] = 11.4f;
        b.Rotate(b.data,b.data+4,b.data+8);
        INFO(b);
    }

   return 0;
}