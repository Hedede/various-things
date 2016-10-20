
namespace xxx {
struct string {
	short size;
	char* data;
};
}

namespace aw {
namespace xxx {
using ::xxx::string;

string MakeString()
{
	return {sizeof("test"), "test"};
}
}
}

using namespace aw;
namespace std {}
using namespace std;

#include <iostream>
int main()
{
	//xxx::string str = xxx::MakeString();
	::xxx::string str = aw::xxx::MakeString();
	cout << str.data;
}
