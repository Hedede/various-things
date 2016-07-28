#include <iostream>
#include "baseclass.h"

class derived : public baseclass {
public:
	derived()
	{
		std::cout << "making derived" << std::endl;
	}

	virtual void w()
	{
		std::cout << "brevnived" << std::endl;
	}
};

derived exemplyar; //глобалспейс, его конструктор вызывается до инт маин, проверил дебагером - вызывается до, и успешно добавляет, сайз вектора 1
