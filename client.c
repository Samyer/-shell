#include "share.h"

void send_cmd(int sock, int pid) {
	char str[MAX_MSG_LENGTH] = {0};
	printf("> ");
	while (fgets(str, MAX_MSG_LENGTH, stdin) == str) {
		if(strncmp(str, END_STRING, strlen(END_STRING)) == 0) break;
		if(send(sock, str, strlen(str)+1, 0) < 0) perro("send");
	}
	kill(pid, SIGKILL);
	printf("Goodbye.\n");
}

void receive(int sock) {
	char buf[MAX_MSG_LENGTH] = {0};
	int filled = 0;	
	while(filled = recv(sock, buf, MAX_MSG_LENGTH-1, 0)) {
		buf[filled] = '\0';
		printf("%s", buf);
		fflush(stdout);		
	}	
	printf("Server disconnected.\n");
}

int main(int argc, char **argv) {
	if(argc != 2) perro("args");

	int sock = socket(AF_INET, SOCK_STREAM, 0);//Uder the TCP,This sentence must use SOCK_STREAM.并且与server端的socket的type参数对应，否则将不能连接上server端 
	if(sock == -1) perro("socket");

	//struct in_addr server_addr;
	//if(!inet_aton(argv[1], &server_addr)) perro("inet_aton");
	
	struct sockaddr_in connection;
	memset(&connection,0,sizeof(connection));//此处相当于给结构体connection初始化为0，可以使用bzero(&connecton,sizeoff(struct sockaddr_in))
	inet_pton(AF_INET,argv[1],&connection.sin_addr.s_addr);
	connection.sin_family = AF_INET;
	//memcpy(&connection.sin_addr, &server_addr, sizeof(server_addr));
	connection.sin_port = htons(PORT);
	if (connect(sock, (struct sockaddr*)&connection, sizeof(connection)) != 0) perro("connect");
	
	int pid;	
	if(pid == fork()) send_cmd(sock, pid);
	else receive(sock);
	
	return 0;
}
