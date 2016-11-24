#include <windows.h>
int main()
{
	int access = GENERIC_READ|GENERIC_WRITE;
	int sharemode = FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE;
	int attrib = FILE_ATTRIBUTE_NORMAL;
	wchar_t* p = L"SOMEFILE";
	CreateFileW(p, access, sharemode, NULL, OPEN_ALWAYS, attrib, NULL);
}
