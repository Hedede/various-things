#include <iostream>

class baseclass {
public:
	baseclass();

	virtual void w() = 0;
};

class derived2 : public baseclass
{
	virtual void w()
	{
		std::cout << "brevnived2" << std::endl;
	}
};

derived2 exemplyar2; //глобалспейс, его конструктор вызывается до инт маин, проверил дебагером - вызывается до, и успешно добавляет, сайз вектора 1
