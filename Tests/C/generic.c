#include <stdio.h>


int main()
{
	puts(_Generic( (int*)0,
				void*: "pointer",
				default: "other")			
			);
}
