#include<iostream>
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
using namespace std;
#include<WinSock2.h>
#include <ws2tcpip.h>
#include<string>

//ham check ten mien//
bool check(string s) {
	for (int i = 0; i < s.length(); i++) {
		if (!((s[i]>='A' && s[i]<='Z')||(s[i]>='a'&&s[i]<='z')||(s[i]=='.')||(s[i]=='-')||(s[i]>='0'&&s[i]<='9')))
			return 0;
	};
	return 1;
}

int main() {
	// khoi tao thu vien//
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	string s;
	cout << "nhap ten mien ";
	getline(cin, s);
	const char* str = s.c_str();

	// phan giai ten mien//

	addrinfo* info;
	SOCKADDR_IN addr;
	if (check(s)) {
		int ret = getaddrinfo(str, "http", NULL, &info);
		if (ret == 0)
		{
			cout << "phan giai ten mien thanh cong!"<<endl;
			addrinfo* point = info;
			while (point != NULL) {
				memcpy(&addr, point->ai_addr, point->ai_addrlen);
				cout << "ip: " << inet_ntoa(addr.sin_addr) << endl;
				point = point->ai_next;
			}
		}
		else
		{
			cout << " khong phan giai dc ten mien";
		}
	}
	else
	{
		cout << "ten mien k hop le";
	}
}