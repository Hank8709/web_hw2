#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "wsock32.lib") 
#include <winsock2.h>
#include<stdio.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//client�� ��ܳ���
int main(void)
{
	WSAData wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // ���\�^�� 0
	SOCKET  server;
	struct sockaddr_in  IP_serv;
	server = socket(AF_INET, SOCK_STREAM, 0);//IPV4 , TCP 
	IP_serv.sin_family = AF_INET;//����IPV4
	IP_serv.sin_addr.s_addr = inet_addr("127.0.0.1");//IP��}
	IP_serv.sin_port = htons(8080);//port���X
	connect(server, (LPSOCKADDR)&IP_serv, sizeof(IP_serv));//�s�u
	//�U�ѳ���
	//send(server, "123", 4, 0);


	char another_game = 'Y';
	char game[3][3] = { {'1','2','3'},{'4','5','6'},{'7','8','9'} };
	bool win = true;
	int choose_player = 0;
	char number =  '0'  ;
	int row = 0, col = 0;
	bool take_turns = false;
	char player1[20], player2[20];
	int c;

	do {
		//��l�ƹC��
		printf("���r�ѹC����l��...\n");
		char game[3][3] = { {'1','2','3'},{'4','5','6'},{'7','8','9'} };
		win = false;

		//��J���a1,2�m�W
		printf("(���n�ϥΪŮ�,�^���䵲��)\n");
		printf("�п�J���a1�m�W:\n");
		scanf("%s", player1);
		//send(server, player1, sizeof(player1), 0);
		printf("�п�J���a2�m�W:\n");
		scanf("%s", player2);
		send(server, player2, sizeof(player2), 0);

		//�H����ܥ����l���a
		srand((unsigned)time(NULL));
		choose_player = rand() % 2 + 1;
		if (choose_player == 1) {
			take_turns = true;
			printf("%s(���a1)�������l(1~9).\n", player1);
		}
		else {
			take_turns = false;
			printf("%s(���a2)�������l(1~9).\n", player2);
		}
		while (!win)
		{
			//��X3x3�C����
			printf("\n");
			printf(" %c | %c | %c \n", game[0][0], game[0][1], game[0][2]);
			printf("---+---+---\n");
			printf(" %c | %c | %c \n", game[1][0], game[1][1], game[1][2]);
			printf("---+---+---\n");
			printf(" %c | %c | %c \n", game[2][0], game[2][1], game[2][2]);
			printf("\n");


			//���a1,���a2���O�U��
			//���l�O�_����
			do {
				printf("%s���l:", take_turns ? player1 : player2);
				//�o�̻ݭn�M���w�İ�(why?)
				if (feof(stdin) || ferror(stdin)) {
					break;
				}
				while ((c = getchar()) != '\n' && c != EOF);
				scanf("%c",&number);
				send(server, &number, sizeof(number), 0);//????????????�p��send���T��char�r����server

				//why:ASCII�X���������Ʀr�t48~
				row = ((int)number - 48 - 1) / 3;
				col = ((int)number - 48 - 1) % 3;
				//printf("%d.%d.%c.\n",row,col,game[row][col]);
				//printf("%c\n",number);
			} while (game[row][col] != number);

			//��s�ѽL
			if (take_turns) {
				game[row][col] = 'X';
			}
			else {
				game[row][col] = 'O';
			}
			if (take_turns) {
				take_turns = false;
			}
			else {
				take_turns = true;
			};
			/*char win_socket[5];
			recv(server, win_socket, sizeof(win_socket), 0);	
			if (win_socket[0] == '1')
				win = true;
			else
				win = false;*/
		}
		
	}
while (toupper(another_game) == 'Y');
	closesocket(server); 
	return 0;
}
	

		

	
	