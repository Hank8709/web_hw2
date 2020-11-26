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

//client端 顯示部分
int main(void)
{
	WSAData wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // 成功回傳 0
	SOCKET  server;
	struct sockaddr_in  IP_serv;
	server = socket(AF_INET, SOCK_STREAM, 0);//IPV4 , TCP 
	IP_serv.sin_family = AF_INET;//接收IPV4
	IP_serv.sin_addr.s_addr = inet_addr("127.0.0.1");//IP位址
	IP_serv.sin_port = htons(8080);//port號碼
	connect(server, (LPSOCKADDR)&IP_serv, sizeof(IP_serv));//連線
	//下棋部分
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
		//初始化遊戲
		printf("井字棋遊戲初始化...\n");
		char game[3][3] = { {'1','2','3'},{'4','5','6'},{'7','8','9'} };
		win = false;

		//輸入玩家1,2姓名
		printf("(不要使用空格,回車鍵結束)\n");
		printf("請輸入玩家1姓名:\n");
		scanf("%s", player1);
		//send(server, player1, sizeof(player1), 0);
		printf("請輸入玩家2姓名:\n");
		scanf("%s", player2);
		send(server, player2, sizeof(player2), 0);

		//隨機選擇先落子玩家
		srand((unsigned)time(NULL));
		choose_player = rand() % 2 + 1;
		if (choose_player == 1) {
			take_turns = true;
			printf("%s(玩家1)首先落子(1~9).\n", player1);
		}
		else {
			take_turns = false;
			printf("%s(玩家2)首先落子(1~9).\n", player2);
		}
		while (!win)
		{
			//輸出3x3遊戲格
			printf("\n");
			printf(" %c | %c | %c \n", game[0][0], game[0][1], game[0][2]);
			printf("---+---+---\n");
			printf(" %c | %c | %c \n", game[1][0], game[1][1], game[1][2]);
			printf("---+---+---\n");
			printf(" %c | %c | %c \n", game[2][0], game[2][1], game[2][2]);
			printf("\n");


			//玩家1,玩家2分別下棋
			//落子是否有效
			do {
				printf("%s落子:", take_turns ? player1 : player2);
				//這裡需要清楚緩衝區(why?)
				if (feof(stdin) || ferror(stdin)) {
					break;
				}
				while ((c = getchar()) != '\n' && c != EOF);
				scanf("%c",&number);
				send(server, &number, sizeof(number), 0);//????????????如何send正確的char字元給server

				//why:ASCII碼錶對應的數字差48~
				row = ((int)number - 48 - 1) / 3;
				col = ((int)number - 48 - 1) % 3;
				//printf("%d.%d.%c.\n",row,col,game[row][col]);
				//printf("%c\n",number);
			} while (game[row][col] != number);

			//更新棋盤

			if (take_turns) {
				game[row][col] = 'X';
			}
			else {
				game[row][col] = 'O';
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
				printf("%s勝利!遊戲結束.", take_turns ? player1 : player2);

				printf("\n");
				printf(" %c | %c | %c \n", game[0][0], game[0][1], game[0][2]);
				printf("---+---+---\n");
				printf(" %c | %c | %c \n", game[1][0], game[1][1], game[1][2]);
				printf("---+---+---\n");
				printf(" %c | %c | %c \n", game[2][0], game[2][1], game[2][2]);
				printf("\n");
			}
			else {

				if (take_turns) {
					take_turns = false;
				}
				else {
					take_turns = true;
				};
			}
			/*char win_socket[5];
			recv(server, win_socket, sizeof(win_socket), 0);	
			if (win_socket[0] == '1')
				win = true;
			else
				win = false;*/
		}
		if (feof(stdin) || ferror(stdin)) {
			break;
		}
		while ((c = getchar()) != '\n' && c != EOF);
		printf("是否開始新遊戲?(y/n)");
		scanf("%c", &another_game);
		
	}
while (toupper(another_game) == 'Y');
	closesocket(server); 
	return 0;
}
	

		

	
	
