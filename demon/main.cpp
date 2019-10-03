#include <winsock2.h>
#include <locale.h>
#include <windows.h>
#include <iostream>
#include <tchar.h>

#include <winsock.h>


//#include "stdafx.h"

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
	if (argnum < 1) {
		wprintf(L"To few arguments!");
		return EXIT_FAILURE;
	}

	int startPort = atoi(argv[1]);

	//std::cout << startPort << "s" << argv[1] << std::endl;

	WSADATA ws;
	WSAStartup(MAKEWORD(2, 0), &ws);

	//int returningProgramValue = EXIT_SUCCESS;

	SOCKET demonSocket;

	DWORD er;

	if ((demonSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		er = WSAGetLastError(); //if some trouble - GET LAST ERROR!
		m_print_error(er);
		//returningProgramValue = EXIT_FAILURE;
		return EXIT_FAILURE;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");
	service.sin_port = htons((u_short)startPort);

	if (bind(demonSocket, (SOCKADDR *)& service, sizeof(service)) == SOCKET_ERROR) {
		er = WSAGetLastError();
		m_print_error(er);
		if (closesocket(demonSocket) == SOCKET_ERROR) {
			er = WSAGetLastError();
			m_print_error(er);
			return EXIT_FAILURE;
		}
		WSACleanup();
		//returningProgramValue = EXIT_FAILURE;
		return EXIT_FAILURE;
	}

	if (listen(demonSocket, SOMAXCONN) == SOCKET_ERROR) {
		er = WSAGetLastError();
		m_print_error(er);
		if (closesocket(demonSocket) == SOCKET_ERROR) {
			er = WSAGetLastError();
			m_print_error(er);
			return EXIT_FAILURE;
		}
		WSACleanup();
		return EXIT_FAILURE;
	}

	int randomCancel = 0;

	wprintf(L"Start listening!");

	while (randomCancel != 12432) {
		SOCKET tryDemon = accept(demonSocket, NULL, NULL);
		if (tryDemon != INVALID_SOCKET) {
			shutdown(tryDemon, 0);
			if (closesocket(tryDemon) == SOCKET_ERROR) {
				er = WSAGetLastError();
				m_print_error(er);
				return EXIT_FAILURE;
			}
			else {
				//wprintf(L"\n?Demon summoned! He came to our world to enslave it! And it starts with this computer!?\n");
				wprintf(L"\nDemon summoned! He came to our world to enslave it! And it starts with this computer!\n");
			}
		}
		shutdown(tryDemon, 0);
		closesocket(tryDemon);
		/*if (closesocket(tryDemon) == SOCKET_ERROR) {
			er = WSAGetLastError();
			m_print_error(er);
		}*/
		randomCancel = rand() % RAND_MAX;
	}

	
	if (closesocket(demonSocket) == SOCKET_ERROR) {
		er = WSAGetLastError();
		m_print_error(er);
		if (closesocket(demonSocket) == SOCKET_ERROR) {
			er = WSAGetLastError();
			m_print_error(er);
			return EXIT_FAILURE;
		}
		WSACleanup();
		return EXIT_FAILURE;
	}

	WSACleanup();

	//return returningProgramValue;
	return EXIT_SUCCESS;

}


