#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <fstream>
#include <sstream>  

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	int userNumber = 6 * 5; //  kullanıcı sayısına göre güncellenir.
	string kullanicilar[30]; 
	string satir;
	ifstream okumadosyası;
	okumadosyası.open("kullanicilar.txt");
	for (int i = 0; i < userNumber; i++) {
		getline(okumadosyası, satir);
		kullanicilar[i] = satir;
		cout << kullanicilar[i] << endl;
	}
	int kullanıcıID;

	
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Winsock baslatilamiyor!" << endl;
		return;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Soket olusturulamiyor! " << endl;
		return;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);

	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		
	char service[NI_MAXSERV];	

	ZeroMemory(host, NI_MAXHOST); 
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	closesocket(listening);

	
	char buf[4096];
	int kontrol = 0;
	while (kontrol == 0)
	{
		ZeroMemory(buf, 4096);

		
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		string username = string(buf, 0, bytesReceived);

		int i = 0;
		while (i < userNumber && kontrol == 0) {
			if (username == kullanicilar[i]) {
				send(clientSocket, "1", 4096, 0);
				kontrol = 1;
				kullanıcıID = i;
			}
			i += 5;
		}

		if (kontrol == 0) {
			send(clientSocket, "0", 4096, 0);
		}


	}
	kontrol = 0;
	while (kontrol == 0)
	{
		ZeroMemory(buf, 4096);

		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		string pass = string(buf, 0, bytesReceived);

		if (pass == kullanicilar[kullanıcıID + 1]) {
			string metin = kullanicilar[kullanıcıID + 2] + " - " + kullanicilar[kullanıcıID + 3] + " - " + kullanicilar[kullanıcıID + 4];
			send(clientSocket, metin.c_str(), 4096, 0);
			kontrol = 1;
		}
		else {
			send(clientSocket, "0", 4096, 0);
		}


	}
	kontrol = 0;
	while (kontrol == 0)
	{
		ZeroMemory(buf, 4096);

		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		string TrNum = string(buf, 0, bytesReceived);

		if (TrNum == "1") {

			ZeroMemory(buf, 4096);
			int bytesReceived = recv(clientSocket, buf, 4096, 0);
			string amount = string(buf, 0, bytesReceived);
			float bakiye = std::stof(kullanicilar[kullanıcıID + 4]);
			bakiye = std::stof(amount) + std::stof(kullanicilar[kullanıcıID + 4]);
			kullanicilar[kullanıcıID + 4] = std::to_string(bakiye);
			remove("kullanicilar.txt");
			ofstream yarat("kullanicilar.txt");
			for (size_t i = 0; i < userNumber; i++)
			{
				yarat << kullanicilar[i] << endl;
			}
			string metin = "Para Yatirma Basarili. Yeni Hesap :" + kullanicilar[kullanıcıID + 4];
			send(clientSocket, metin.c_str(), 4096, 0);

		}

		else if (TrNum == "2") {

			ZeroMemory(buf, 4096);
			int bytesReceived = recv(clientSocket, buf, 4096, 0);
			string amount = string(buf, 0, bytesReceived);
			float bakiye = std::stof(kullanicilar[kullanıcıID + 4]);
			if (std::stof(amount) > bakiye) {
				send(clientSocket, "Yetersiz bakiye! Lutfen Hesabinizi Kontrol Edin.", 4096, 0);
			}
			else
			{
				bakiye = std::stof(kullanicilar[kullanıcıID + 4]) - std::stof(amount);
				kullanicilar[kullanıcıID + 4] = std::to_string(bakiye);
				remove("kullanicilar.txt");
				ofstream yarat("kullanicilar.txt");
				for (size_t i = 0; i < userNumber; i++)
				{
					yarat << kullanicilar[i] << endl;
				}
				string metin = "Geri Cekilme Basarili. Yeni Hesap : " + kullanicilar[kullanıcıID + 4];
				send(clientSocket, metin.c_str(), 4096, 0);

			}

		}

		else if (TrNum == "3") {
			int alici = 0;
			int k = 3;
			int kesinti = 0;
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(clientSocket, buf, 4096, 0);
			string accnNum = string(buf, 0, bytesReceived);
			while (k < userNumber && alici == 0)
			{
				if (accnNum == kullanicilar[k]) {
					alici = k;
				}
				else
				{
					k += 5;
				}

			}
			if (alici != 0) {
				send(clientSocket, "1", 4096, 0);
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(clientSocket, buf, 4096, 0);
				string amount = string(buf, 0, bytesReceived);
				float bakiye = std::stof(kullanicilar[kullanıcıID + 4]);
				if (kullanicilar[kullanıcıID + 2] != kullanicilar[k - 1])
				{
					kesinti = 5;
				}

				if ((std::stof(amount)) + kesinti > bakiye) {
					send(clientSocket, "Yetersiz bakiye! Lutfen Hesabinizi Kontrol Edin.", 4096, 0);
				}
				else
				{

					bakiye = std::stof(kullanicilar[kullanıcıID + 4]) - (std::stof(amount) + kesinti);
					kullanicilar[kullanıcıID + 4] = std::to_string(bakiye);
					float gonderilenbakiye = std::stof(kullanicilar[k + 1]);
					gonderilenbakiye = std::stof(kullanicilar[k + 1]) + std::stof(amount);
					kullanicilar[k + 1] = std::to_string(gonderilenbakiye);
					remove("kullanicilar.txt");
					ofstream yarat("kullanicilar.txt");
					for (size_t i = 0; i < userNumber; i++)
					{
						yarat << kullanicilar[i] << endl;
					}
					string metin = "Para Gonder Islemi Basarili. Yeni Hesap : " + kullanicilar[kullanıcıID + 4  ];
					send(clientSocket, metin.c_str(), 4096, 0);

				}
			}
			else
			{
				send(clientSocket, "Hesap bulunamadi! Lutfen tekrar deneyin.", 4096, 0);
			}

		}

		else if (TrNum == "4") {
			kontrol = 1;

		}

	}
	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}