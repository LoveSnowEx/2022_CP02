#include "client.h"

int client() {
	//socket的建立
	int sockfd = -1;
	if ((sockfd = socket(AF_INET , SOCK_STREAM , 0)) < 0){
		printf("Fail to create a socket.");
	}

	//socket的連線
	struct sockaddr_in serv_addr;
	bzero(&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8080);
	serv_addr.sin_addr.s_addr = inet_addr("192.168.100.22");

	int err = -1;
	if((err = connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))) == -1){
		printf("Connection error");
	}

	//Send a message to server
	char *message = "Hi there";
	char receiveMessage[1024] = {};
	send(sockfd,message,sizeof(message),0);
	recv(sockfd,receiveMessage,sizeof(receiveMessage),0);

	printf("%s",receiveMessage);
	printf("close Socket\n");
	close(sockfd);

	return 0;
}
