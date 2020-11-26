#pragma comment(lib, "wsock32.lib") 
#include <winsock2.h>
#include<stdio.h>

//server端 判斷輸贏
int main(void)
{
	WSAData wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // 成功回傳 0
	SOCKET client[2], server;
	struct sockaddr_in IP_cli, IP_serv;
	server = socket(AF_INET, SOCK_STREAM, 0);//IPV4 , TCP 
	IP_serv.sin_family = AF_INET;//接收IPV4
	IP_serv.sin_addr.s_addr = 0;//IP位址
	IP_serv.sin_port = htons(8080);//port號碼
	bind(server, (LPSOCKADDR)&IP_serv, sizeof(IP_serv));//server,上述結構設定好bind在一起,設定大小
	listen(server, 5);//數字只是一個監聽狀態
	int len_of_client = sizeof(IP_cli);

	char  player[10][1024];

	int people = 0;
	//用multithread去控制有幾個玩家連線進來開棋局
	while (people<5)//看有幾個玩家連線進來,上限2個
	{
		
		client[people] = accept(server, (LPSOCKADDR)&IP_cli, &len_of_client);//監聽到的時候連接近來
		recv(client[people],player[people],sizeof(player[people]),0);
		printf("cleint_id %d %s\n", people,player[people]);
		people++;
	}

	char position[1024];
	int round = 0;
	bool win = false;
	char game[3][3] = { {'1','2','3'},{'4','5','6'},{'7','8','9'} };
	int row = 0, col = 0;
	while (!win)//判斷遊戲輸贏,單一棋局
	{
		if(round%2==0)
		{ 
			recv(client[0], position, sizeof(position), 0);//玩家1下這步棋
			row = ((int)position[1] - 48 - 1) / 3;
			col = ((int)position[1] - 48 - 1) % 3;
			//round++;
		}
		else
		{
			recv(client[1], position, sizeof(position), 0);//玩家2下這步棋
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
	
	//client連進來了

	//遊戲

	closesocket(server);

	for(int i = 0 ;i < 2 ;i++)
	closesocket(client[i]);

	return 0;
}
