// t1-v2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <winsock2.h>
#include <locale.h>
#include <windows.h>
#include <iostream>

#pragma warning(disable : 4996)

void m_print_error(DWORD er) {

	setlocale(LC_ALL, "Russian");

	wchar_t ErrorMessage[1000];
	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		er,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)ErrorMessage, sizeof(ErrorMessage),
		NULL);
	_tprintf(TEXT("\nОШИБКА: %s\n"), ErrorMessage);

}

int main(int argnum, char* argv[])
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 0), &ws);

	setlocale(LC_ALL, "Russian");

	if (argnum < 2)
	{
		printf("Слишком мало аргументов в командной строке.");
		return -1;
	}

	DWORD er;

	hostent* host = gethostbyname(argv[1]);

	int first_be = 0;
	
	if (host != NULL)
	{
		wprintf(L"Каноничное имя: %S\n", host->h_name);
		for (char** pc = host->h_aliases; *pc; pc++)
			wprintf(L"Синоним: %S\n", *pc);
		first_be = 1;
	}
	else
	{
		er = WSAGetLastError();
		m_print_error(er);
	}
	
	long addr = inet_addr(argv[1]);
	
	host = gethostbyaddr((char*)&addr, 4, AF_INET);
	if (host != NULL )
	{
		for (char** pc = host->h_addr_list; *pc; pc++)
			wprintf(L"IP-адрес: %S\n", inet_ntoa(*(in_addr*)(pc)));
		//wprintf(L"IP-адрес: %S\n", inet_ntoa(*(in_addr*)(host->h_addr_list[0])));
		wprintf(L"Каноничное имя: %S\n", host->h_name);
		for (char** pc = host->h_aliases; *pc; pc++)
			wprintf(L"Синоним: %S\n", *pc);
	}
	else
	{
		if (first_be != 1) {
			er = WSAGetLastError();
			m_print_error(er);
		}
	}
	


	WSACleanup();

    return 0;
}

/*using std::cin;
using std::cout;


struct in_addr {
union {
struct {
u_char s_b1, s_b2, s_b3, s_b4;
} S_un_b;
struct {
u_short s_w1, s_w2;
} S_un_w;
u_long S_addr;
} S_un;
} in_addr;
typedef struct in_addr IN_ADDR;

typedef struct hostent {
char* h_name;
char** h_aliases;
short h_addrtype;
short h_length;
char** h_addr_list;
} hostent;
*/
/*
long ip = inet_addr("10.0.0.31");
if (ip == INADDR_NONE)
{
wprintf(L"Неправильный формат IP-адреса\n");
// Обработка ошибки
m_print_error();
}
struct in_addr ips;
ips.s_addr = ip;
wprintf(L"IP-адрес:\t%d.%d.%d.%d\n",
ips.S_un.S_un_b.s_b1, ips.S_un.S_un_b.s_b2,
ips.S_un.S_un_b.s_b3, ips.S_un.S_un_b.s_b4);

*/

/*if (gethostname(argv[1], sizeof(argv[1])) != SOCKET_ERROR)
{
// Обработка ошибки
er = WSAGetLastError();
m_print_error(er);
}
wprintf(L"Название данного хоста: %S\n", HostName);
*/