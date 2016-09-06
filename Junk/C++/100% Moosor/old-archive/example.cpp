#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <cassert>

void fun1(void){
    std::cout<<"inside fun1\n";
}

int fun2(){
    std::cout<<"inside fun2\n";
    return 2;
}

int fun3(int a){
    std::cout<<"inside fun3\n";
    return a;
}

std::vector<int> fun4(){
    std::cout<<"inside fun4\n";
    std::vector<int> v(4,100);
    return v;
}
using crea0 = object* (*) (int,char);

// every function pointer will be stored as this type
typedef void (*voidFunctionType)(void); 

struct Interface{

    std::map<std::string,std::pair<voidFunctionType,std::type_index>> m1;

    template<typename T>
    void insert(std::string s1, T f1){
        auto tt = std::type_index(typeid(f1));
        m1.insert(std::make_pair(s1,
                        std::make_pair((voidFunctionType)f1,tt)));
    }

    template<typename T,typename... Args>
    T searchAndCall(std::string s1, Args&&... args){
        auto mapIter = m1.find(s1);
        /*chk if not end*/
        auto mapVal = mapIter->second;

	std::cout << std::type_index(typeid(T(*)(Args ...))).hash_code() << std::endl;

        // auto typeCastedFun = reinterpret_cast<T(*)(Args ...)>(mapVal.first); 
        auto typeCastedFun = (T(*)(Args ...))(mapVal.first); 

        //compare the types is equal or not
        assert(mapVal.second == std::type_index(typeid(typeCastedFun)));
        return typeCastedFun(std::forward<Args>(args)...);
    }
};

int main(){
    Interface a1;
    a1.insert("fun1",fun1);
    a1.insert("fun2",fun2);
    a1.insert("fun3",fun3);
    a1.insert("fun4",fun4);

    using crea1 = object* (*) (int,char);
    using crea2 = object* (*) (int,char);
    crea1 cre = [] (int x, char y) {return new object(x,y);};

    std::cout << std::type_index(typeid(crea1)).hash_code() << std::endl;
    std::cout << std::type_index(typeid(crea2)).hash_code() << std::endl;
    std::cout << std::type_index(typeid(cre)).hash_code() << std::endl;

    a1.insert("create", cre);

    a1.searchAndCall<void>("fun1");
    int retVal = a1.searchAndCall<int>("fun3",2);
    a1.searchAndCall<int>("fun2");
    auto temp = a1.searchAndCall<std::vector<int>>("fun4");

    auto obj = a1.searchAndCall<object*>("create", 21, 'c');
    return 0;
}
