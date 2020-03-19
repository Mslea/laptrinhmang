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
		if (!((s[i]>='a'&&s[i]<='z')||(s[i]=='.')||(s[i]=='-')))
			return 0;
	};
	return 1;
}

int main() {
// khoi tao thu vien//
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	addrinfo* info;
	SOCKADDR_IN addr;
	string s;
	cout << "nhap ten mien ";
	getline(cin, s);
	const char* str = s.c_str();
	if (check(s)) {
		int ret = getaddrinfo("hust.edu.vn", "http", NULL, &info);
		if (ret == 0)
		{
			memcpy(&addr, info->ai_addr, info->ai_addrlen);
			cout << "Phan giai ten mien thanh cong\n";
			cout << "dia chi ip :" << inet_ntoa(addr.sin_addr);
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