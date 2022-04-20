#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "127.0.0.1";			
	int port = 54000;						

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Winsock baslatılamiyor, Err #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Soket olusturulamiyor, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Sunucuya baglanilamiyor, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	
	char buf[4096];
	string userInput;
	string amount;
	string accNum;
	cout << "TCP Banka Sistemi " << endl;
	int kontrol = 0;

	do
	{

		cout << "Lutfen Isminizi giriniz : ";
		getline(cin, userInput);


		if (userInput.size() > 0)		
		{
			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{

					if (string(buf, 0, bytesReceived) == "1") {
						kontrol = 1;
						cout << "Sifre Girme Adimina Yonlendiriliyorsunuz..." << endl;
					}
					else {
						cout << "Gecersiz kullanici adı. Tekrar deneyin." << endl;
					}
				}
			}
		}

	} while (kontrol == 0);
	kontrol = 0;
	do
	{
		cout << "Lutfen Kullanici Sifrenizi Giriniz : ";
		getline(cin, userInput);



		if (userInput.size() > 0)
		{

			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{

				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{

					if (string(buf, 0, bytesReceived) != "0") {
						cout << "Giris basarili. Lutfen bekleyin ..." << endl;
						kontrol = 1;
						cout << "HOSGELDINIZ" << endl;
						cout << " Banka Adi - Hesap Numarasi - Bakiye " << endl;
						cout << string(buf, 0, bytesReceived) << endl;
					}
					else {
						cout << "Yanlıs sifre.Tekrar deneyin." << endl;
					}
				}
			}
		}

	} while (kontrol == 0);
	kontrol = 0;
	do
	{

		cout << "1- Hesabiniza Para Yatirma> " << endl;
		cout << "2- Hesabinizdan Para Cekme > " << endl;
		cout << "3- Baska Bir Hesaba Para Transferi > " << endl;
		cout << "4- CIKIS > " << endl;
		cout << " Lutfen yapmak istediginiz islem numarasini giriniz : ";
		getline(cin, userInput);
		int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
		if (userInput == "1")
		{
			cout << "Yatirilacak Tutari Girin :  ";
			getline(cin, amount);
			int sendResult = send(sock, amount.c_str(), amount.size() + 1, 0);
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0)
			{
				cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;

			}
		}
		else if (userInput == "2")
		{
			cout << "Cekilecek Tutari Girin : ";
			getline(cin, amount);
			int sendResult = send(sock, amount.c_str(), amount.size() + 1, 0);
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0)
			{
				cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;

			}
		}
		else if (userInput == "3")
		{
			cout << "Para Gonderilecek Hesap Numarasini Girin : ";
			getline(cin, accNum);
			int sendResult = send(sock, accNum.c_str(), accNum.size() + 1, 0);
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0)
			{
				if (string(buf, 0, bytesReceived) == "1")
				{
					cout << "Dikkat! Baska Bir Bankaya Havale Yapiyorsaniz 5 TL EFT Ucreti Alinacaktir.";
					cout << "Gonderilecek Para Miktarini Giriniz : ";
					getline(cin, amount);
					int sendResult = send(sock, amount.c_str(), amount.size() + 1, 0);
					ZeroMemory(buf, 4096);
					int bytesReceived = recv(sock, buf, 4096, 0);
					if (bytesReceived > 0)
					{
						cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;

					}
				}
				else
				{
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				}

			}
		}
		else if (userInput == "4") {
			kontrol = 1;
		}

	} while (kontrol == 0);

	
	closesocket(sock);
	WSACleanup();
}