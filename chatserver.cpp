// ChatServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

DWORD WINAPI ClientThread(LPVOID);

// Luu thong tin client dang nhap thanh cong
char* ids[64];
SOCKET clients[64];
int numClients = 0;

CRITICAL_SECTION cs;

int main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	InitializeCriticalSection(&cs);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	listen(listener, 5);

	while (1)
	{
		printf("Dang cho cac ket noi...\n");
		SOCKET client = accept(listener, NULL, NULL);
		printf("Ket noi moi: %d", client);

		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}

	DeleteCriticalSection(&cs);
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET*)lpParam;

	char buf[256];
	int ret;

	char cmd[16], id[32], tmp[32];
	char sendBuf[256];

	BOOL isRegistered = FALSE;

	const char* helloMsg = "Dang nhap theo cu phap \"[client_id:] [your_id]\".\n";
	send(client, helloMsg, strlen(helloMsg), 0);

	while (1)
	{
		// Nhan du lieu
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
		{
			closesocket(client);
			return 0;
		}

		buf[ret] = 0;
		printf("Received: %s\n", buf);

		if (!isRegistered) // Chua dang nhap
		{
			// Kiem tra cu phap
			ret = sscanf(buf, "%s %s %s", cmd, id, tmp);
			if (strcmp(cmd, "CONNECT") == 0)
			{
				if (ret == 2)
				{
					int i;
					for (i = 0; i < numClients; i++)
						if (strcmp(id, ids[i]) == 0)
							break;

					if (i < numClients)
					{
						const char* errorMsg = "CONNECT ERROR ID da duoc su dung. Hay chon ID khac.\n";
						send(client, errorMsg, strlen(errorMsg), 0);
					}
					else
					{
						const char* okMsg = "CONNECT OK.\n";
						send(client, okMsg, strlen(okMsg), 0);
						isRegistered = true;

						EnterCriticalSection(&cs);
						ids[numClients] = id;
						clients[numClients] = client;
						numClients++;
						LeaveCriticalSection(&cs);
					}					
				}
				else
				{
					const char* errorMsg = "CONNECT ERROR Sai cu phap lenh CONNECT.\n";
					send(client, errorMsg, strlen(errorMsg), 0);
				}
			}
			else
			{
				const char* errorMsg = "ERROR Lenh khong hop le.\n";
				send(client, errorMsg, strlen(errorMsg), 0);
			}			
		}
		else // Da dang nhap
		{
			ret = sscanf(buf, "%s", cmd);

			if (strcmp(cmd, "SEND") == 0)
			{
				char target[32];
				ret = sscanf(buf + strlen(cmd) + 1, "%s", target);

				if (ret == -1)
				{
					const char* errorMsg = "SEND ERROR Sai cu phap lenh SEND.\n";
					send(client, errorMsg, strlen(errorMsg), 0);
				}
				else
				{
					if (strcmp(target, "ALL") == 0)
					{
						/*sprintf(sendBuf, "MESSAGE_ALL %s %s", id, buf + strlen(cmd) + 1);
						for (int i = 0; i < numClients; i++)
							if (clients[i] != client)
								send(clients[i], sendBuf, strlen(sendBuf), 0);*/
					}
					else
					{
						/*sprintf(sendBuf, "MESSAGE %s %s", id, buf + strlen(cmd) + 1);
						for (int i = 0; i < numClients; i++)
							if (strcmp(ids[i], cmd + 1) == 0)
								send(clients[i], sendBuf, strlen(sendBuf), 0);*/
					}
				}
			}
		}
		
	}

}