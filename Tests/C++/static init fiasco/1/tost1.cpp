#include <vector>

class baseclass {
public:
	baseclass();

	virtual void w() = 0;
};

std::vector<baseclass*> vectar;

baseclass::baseclass()
{
	vectar.push_back( this );
}

void iterator() {
	for( int i = 0, e = vectar.size(); i < e; i++ ) //вызывается из инт маин после конструктора, сайз вектора 0
	{
		vectar[i]->w();
	}
}

int main()
{
	iterator();
}
