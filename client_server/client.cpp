#include <iostream>
#include <winsock2.h>
#include <tchar.h>
#include <winsock.h>
#include <locale.h>

#include <iostream>
#include <fstream>

#pragma warning(disable : 4996)

using std::ifstream;
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

bool resultOfServerWork(char* executingFileName, char* openFileName, int id, SOCKET socketClient, sockaddr_in* sockAddrName, int port) {
	bool returningValue = false;

	FILE* sendingFile;
	sendingFile = fopen(openFileName, "rt");

	//stream for read from file
	ifstream fileSend;
	ofstream fileCreate;
	//buffer of simbols
	char buffer[1000];
	DWORD er;

	//create file for sending
	fileCreate.open(openFileName);
	fileCreate << "Client send this file. His id is " << id << std::endl;

	
	if (connect(socketClient, (sockaddr*)sockAddrName, sizeof(sockaddr_in)) != 0) {
		er = GetLastError();
		myPrintError(er);
		printf("Connection Error! Port Number %d\n", port); //if we hav problem , we return connection error
		returningValue = false;
	}
	else {
		//open file
		fileSend.open(executingFileName);

		//include first part of file in buffer
		fileSend >> buffer;
		
		//while we send somethink from file, we repeat this sends
		while (fread(buffer, sizeof(char), sizeof(buffer), sendingFile) >= sizeof(buffer)) {
			send(socketClient, buffer, sizeof(buffer), 0);
			//fileSend >> buffer;
			//fread(buffer, sizeof(char), sizeof(buffer), sendingFile);
		}
		if (recv(socketClient, buffer, sizeof(buffer), 0)) {
			cout << buffer;
			returningValue = true;
		}
		else {
			cout << "Error. Somethink goes wrong! (RECV - 1)" << std::endl;
			returningValue = false;
		}
		//send second file
		fileSend.open(openFileName);
		fileSend >> buffer;
		while (send(socketClient, buffer, sizeof(buffer), 0) >= sizeof(buffer)) {
			fileSend >> buffer;
		}
		if (recv(socketClient, buffer, sizeof(buffer), 0)) {
			cout << buffer;
			returningValue = true;
		}
		else {
			cout << "Error. Somethink goes wrong! (RECV - 2)" << std::endl;
			returningValue = false;
		}

	}
		
	//if (connect(socketClient, addrName, sizeof(addrName)) == )
	
	return returningValue;
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

	//client socket
	SOCKET clientSock;
	//port for connecting with server
	int startPort = atoi(argv[1]);
	char* serverAddress = argv[2];

	if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) { //create client socket
		er = WSAGetLastError(); //if some trouble - GET LAST ERROR!
		myPrintError(er);
		return EXIT_FAILURE;
	}

	sockaddr_in sockStruct; //our socket structure
	memset(&sockStruct, 0, sizeof(sockStruct)); //set memory for our socket
	sockStruct.sin_family = AF_INET;
	sockStruct.sin_addr.s_addr = inet_addr(serverAddress);
	sockStruct.sin_port = htons((u_short)startPort);

	if (resultOfServerWork("notepad2.exe", "openName.txt", 1, clientSock, &sockStruct, startPort) != false) {
		cout << "All normal!";
	}
	else {
		cout << "Somethink goes wrong after try to connect server!" << std::endl;
	}

	system("pause");
	return EXIT_SUCCESS;

}