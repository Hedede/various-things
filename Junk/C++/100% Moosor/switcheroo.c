int main()
{
	int x = 0;
	goto LOL;
	
	if (x == 2 ||
	   (x=3, x)) {
LOL:
		x = 5;
	}
}
