//#include "stdafx.h"
#include <winsock2.h>

void printErrorMessage() {
	wchar_t ErrorMessage[1000];
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)ErrorMessage, sizeof(ErrorMessage),
		NULL);
	wprintf(L"\nŒÿ»¡ ¿: %s\n", ErrorMessage);}int main() {	return 0;}