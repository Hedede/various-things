#include <windows.h>

/*
 * This function is supposed to be disassembled and then placed
 * into executable instead of one of unused functions
 */
void dohook()
{
	auto ptr = LoadLibrary(TEXT("testlib.dll"));
	auto mod = GetModuleHandle(TEXT("testlib.dll"));
	int(*fun)()  = (int(*)())GetProcAddress(mod, TEXT("aw_main"));
	fun();
}
int main()
{
	dohook();
}
