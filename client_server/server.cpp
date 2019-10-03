#include <iostream>
#include <winsock2.h>
#include <tchar.h>
#include <winsock.h>
#include <locale.h>
#include <cstring>
#include <string.h>
#include <iostream>
#include <fstream>

#pragma warning(disable : 4996)

using std::ofstream;
using std::cout;
using std::cin;

//function for print error message
void myPrintError(DWORD er) {

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

bool startMyProcess(char* processName, char* processCommandLine) {
	bool returningValue = false;
	DWORD er;

	STARTUPINFO cif; //startup information
	ZeroMemory(&cif, sizeof(STARTUPINFO));//000.. for si
	cif.cb = sizeof(cif);
	PROCESS_INFORMATION pi; // information about process
	ZeroMemory(&pi, sizeof(pi));
	
	if ((returningValue = CreateProcess(processName, 
				processCommandLine, 
				NULL,		
				NULL, 
				FALSE, 
				NULL, 
				NULL, 
				NULL,
				&cif, &pi)) == TRUE) {
		return returningValue;
	}
	else {
		er = WSAGetLastError();
		myPrintError(er);
		returningValue = false;
		return returningValue;
	}
}

char* mstrcat(char *str1, const char *str2)
{
	char *res = str1;
	while (*str1)
		++str1;
	while (*str1++ = *str2++);
	return res;
}


int main(int argnum, char* argv[]) {

	if (argnum < 1) {
		wprintf(L"To few arguments!");
		return EXIT_FAILURE;
	}

	WSADATA ws;
	WSAStartup(MAKEWORD(2, 0), &ws);

	//error message
	DWORD er;

	//server socket
	SOCKET serverSock;
	//port for server
	int startPort = atoi(argv[1]);
	char* serverAddress = argv[2];

	if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) { //create server socket
		er = WSAGetLastError(); //if some trouble - GET LAST ERROR!
		myPrintError(er);
		return EXIT_FAILURE;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(serverAddress);
	service.sin_port = htons((u_short)startPort);

	if (bind(serverSock, (sockaddr*)& service, sizeof(service)) == SOCKET_ERROR) { //bind serverSock
		er = WSAGetLastError();
		myPrintError(er);
		if (closesocket(serverSock) == SOCKET_ERROR) { //print error when close socket
			er = WSAGetLastError();
			myPrintError(er);
			return EXIT_FAILURE;
		}
		WSACleanup();
		return EXIT_FAILURE;
	}

	//server start listen net
	if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR) {
		er = WSAGetLastError();
		myPrintError(er);
		if (closesocket(serverSock) == SOCKET_ERROR) {
			er = WSAGetLastError();
			myPrintError(er);
			return EXIT_FAILURE;
		}
		WSACleanup();
		return EXIT_FAILURE;
	}

	wprintf(L"Start server:");

	int randomCancel = 0;

	//structure == information about client
	sockaddr clientInformation;

	//buffer for "chat" with server
	char serverBuffer[1000];

	char* processFileName = "executing.exe";
	char* startingProcessName = "..\\";
	char* openFileName = "openFile";

	//create file name for create process
	char tempFileName[256];
	memset(tempFileName, 0, sizeof(256));
	sprintf(tempFileName, "%s%s", openFileName, ".txt");

	//create file name for execute programm
	char tempExecutingFileName[256];
	memset(tempExecutingFileName, 0, sizeof(256));
	sprintf(tempExecutingFileName, "%s%s", "..\\", processFileName);


	//our output file where we will print snd message
	ofstream fileOut;

	//Random while-true loop
	while (randomCancel != 12432) {
		fileOut.open(processFileName);
		SOCKET serverTryAccept = accept(serverSock, NULL, NULL/*sizeof(clientInformation)*/);
																				//todo: think here!
		if (serverTryAccept != INVALID_SOCKET) {
			cout << "Accept" << std::endl;
			
			//work with client
			//read information from buffer and print in executing file
			while (recv(serverTryAccept, serverBuffer, sizeof(serverBuffer), 0) >= sizeof(serverBuffer)) {
				fileOut << serverBuffer;
			}
			fileOut.close(); //close file
			
							 //create message for client.
			serverBuffer[1000] = { 0 };
			strcpy(serverBuffer, "The file has been sent completely. The socket will be closed.");
			//send message to client what all normal
			send(serverTryAccept, serverBuffer, sizeof(serverBuffer), 0);
			
			//work with client
			//read opening file
			fileOut.open(openFileName);
			while (recv(serverTryAccept, serverBuffer, sizeof(serverBuffer), 0) >= sizeof(serverBuffer)) {
				fileOut << serverBuffer;
			}
			fileOut.close();

			//create message for client.
			serverBuffer[1000] = { 0 };
			strcpy(serverBuffer, "The file has been sent completely. The socket will be closed.");
			//send message to client what all normal
			send(serverTryAccept, serverBuffer, sizeof(serverBuffer), 0);
			
			//information for start process
			
			//try to create process
			if (startMyProcess(tempExecutingFileName, tempFileName) == TRUE) {
				printf("Server works normal!");
			}
			else {
				printf("Error. Server can't start process.");
			}

			//close server socket
			shutdown(serverTryAccept, 0);
			closesocket(serverTryAccept); //check errors????
		}
		else {
			er = WSAGetLastError();
			myPrintError(er);
			WSACleanup();
			return EXIT_FAILURE;
		}
		randomCancel = rand() % RAND_MAX;
	}

	shutdown(serverSock, 0);
	closesocket(serverSock);

	WSACleanup();

	system("pause");
	return EXIT_SUCCESS;
}