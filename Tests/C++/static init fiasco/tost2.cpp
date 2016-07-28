#include <iostream>

class baseclass {
public:
	baseclass();

	virtual void w() = 0;
};

class derived : public baseclass
{
	virtual void w()
	{
		std::cout << "brevnived" << std::endl;
	}
};

derived exemplyar; //глобалспейс, его конструктор вызывается до инт маин, проверил дебагером - вызывается до, и успешно добавляет, сайз вектора 1
