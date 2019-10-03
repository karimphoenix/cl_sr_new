// sockets_task2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <winsock2.h>
#include <locale.h>
#include <windows.h>
#include <iostream>

#include <winsock.h>

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
	_tprintf(TEXT("\nError: %s\n"), ErrorMessage);

}


int main(int argnum, char* argv[])
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2,0), &ws);

	hostent* host = gethostbyname(argv[1]); //my host

	sockaddr_in SockStruct; //our socket structure
	memset(&SockStruct, 0, sizeof(SockStruct)); //set memory for our socket

	printf(host->h_name);
	printf("\n");

	//initialise our sturcture
	SockStruct.sin_family = AF_INET; 
	SockStruct.sin_addr.s_addr = *(ULONG*)host->h_addr_list[0];
	
	int i;
	DWORD er;

	struct servent* service;

	//try to scan port in this loop
	for (i = atoi(argv[2]); i < atoi(argv[3]) + 1; ++i) {
		SOCKET tsocket; //our soceket
		//try to create socket
		if ((tsocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			er = WSAGetLastError(); //if some trouble - GET LAST ERROR!
			m_print_error(er);
		}
		//try to connect to next port
		SockStruct.sin_port = htons(i);
		if (connect(tsocket, (sockaddr*)&SockStruct, sizeof(SockStruct)) != 0) { 
			printf("Connection Error! Port Number %d", i); //if we hav problem , we return connection error
			er = GetLastError();
			m_print_error(er);

			/*wchar_t Mess[1000];
			swprintf(Mess, L"Ошибка Connect() %S:%d",
				inet_ntoa(SockStruct.sin_addr), htons(i));
			WriteErrorMessage(Mess, er);*/
		}
		else {
			printf("Normal port: %d\n", i);
			service = getservbyport(htons(i), 0);
			if (service != NULL) {
				wprintf(L"Service:\t%S\t%d\t%S\n\n",
					service->s_name, htons(service->s_port),
					service->s_proto);
			}
			else {
				printf("Unknown service! \n\n");
			}
		}

		closesocket(tsocket);
		
	}
	


	WSACleanup();

	system("pause");

    return EXIT_SUCCESS;
}