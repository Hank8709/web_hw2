#pragma comment(lib, "wsock32.lib") 
#include <winsock2.h>
#include<stdio.h>

//server�� �P�_��Ĺ
int main(void)
{
	WSAData wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // ���\�^�� 0
	SOCKET client[2], server;
	struct sockaddr_in IP_cli, IP_serv;
	server = socket(AF_INET, SOCK_STREAM, 0);//IPV4 , TCP 
	IP_serv.sin_family = AF_INET;//����IPV4
	IP_serv.sin_addr.s_addr = 0;//IP��}
	IP_serv.sin_port = htons(8080);//port���X
	bind(server, (LPSOCKADDR)&IP_serv, sizeof(IP_serv));//server,�W�z���c�]�w�nbind�b�@�_,�]�w�j�p
	listen(server, 5);//�Ʀr�u�O�@�Ӻ�ť���A
	int len_of_client = sizeof(IP_cli);

	char  player[10][1024];

	int people = 0;
	//��multithread�h����X�Ӫ��a�s�u�i�Ӷ}�ѧ�
	while (people<1)//�ݦ��X�Ӫ��a�s�u�i��,�W��2��
	{
		
		client[people] = accept(server, (LPSOCKADDR)&IP_cli, &len_of_client);//��ť�쪺�ɭԳs�����
		recv(client[people],player[people],sizeof(player[people]),0);
		printf("cleint_id %d %s\n", people,player[people]);
		people++;
	}

	char position[1024];
	int round = 0;
	bool win = false;
	char game[3][3] = { {'1','2','3'},{'4','5','6'},{'7','8','9'} };
	int row = 0, col = 0;
	while (!win)//�P�_�C����Ĺ,��@�ѧ�
	{
		if(round%2==0)
		{ 
			recv(client[0], position, sizeof(position), 0);//���a1�U�o�B��
			row = ((int)position[1] - 48 - 1) / 3;
			col = ((int)position[1] - 48 - 1) % 3;
			//round++;
		}
		else
		{
			recv(client[1], position, sizeof(position), 0);//���a2�U�o�B��
			row = ((int)position[1] - 48 - 1) / 3;
			col = ((int)position[1] - 48 - 1) % 3;
			round++;
		}
		if (round % 2 == 0) {
			game[row][col] = 'X';
			round++;
		}
		else {
			game[row][col] = 'O';
			round++;
		}
			
		if ((game[0][0] == game[0][1] && game[0][1] == game[0][2]) ||
			(game[1][0] == game[1][1] && game[1][1] == game[1][2]) ||
			(game[2][0] == game[2][1] && game[2][1] == game[2][2]) ||
			(game[0][0] == game[1][0] && game[1][0] == game[2][0]) ||
			(game[0][1] == game[1][1] && game[1][1] == game[2][1]) ||
			(game[0][2] == game[1][2] && game[1][2] == game[2][2]) ||
			(game[0][0] == game[1][1] && game[1][1] == game[2][2]) ||
			(game[0][2] == game[1][1] && game[1][1] == game[2][0])
			) {
			win = true;
			
			
		}
		if(win == true)
		{
			send(client[0], "1", sizeof(char), 0);
		}
		else
			send(client[0], "0", sizeof(char), 0);
		
	}
	
	//client�s�i�ӤF

	//�C��

	closesocket(server);

	for(int i = 0 ;i < 2 ;i++)
	closesocket(client[i]);

	return 0;
}