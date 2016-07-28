#include <iostream>
#include "baseclass.h"

class derived2 : public baseclass
{
	virtual void w()
	{
		std::cout << "brevnived2" << std::endl;
	}
};

derived2 exemplyar2; //глобалспейс, его конструктор вызывается до инт маин, проверил дебагером - вызывается до, и успешно добавляет, сайз вектора 1
