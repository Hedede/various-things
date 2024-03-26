#include "windows.h"
#include "gl/gl.h"
#include "gl/glu.h"

int main(int argc, char** argv)
{
	auxInitDisplayMode(AUX_RGB);
	auxInitPosition (0, 0, 500, 500);
	auxInitWindow ("Step1");
	return 0;
}
