#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(void)
{
	int sockfd = 0, n = 0, num = 0;

	char cont = 'y';
	struct sockaddr_in serv_addr;

	char recvBuff[1024];
	char sendBuff[1025];
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(recvBuff, '0', sizeof(recvBuff));
	while(cont == 'y'){
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			printf("\n Error : Could not create socket \n");
			return 1;
		}
		printf("start\n");
		if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		{
			printf("\n Error : Connect Failed \n");
			return 1;
		}

		printf("Enter: ");
		gets(sendBuff);
		printf("len %d sending %s", strlen(sendBuff), sendBuff);

		if ((send(sockfd, sendBuff, strlen(sendBuff), 0)) == -1)
		{
			fprintf(stderr, "Failure Sending Message\n");
			close(sockfd);
		}

		num = recv(sockfd, recvBuff, sizeof(recvBuff), 0);
		if (num <= 0)
		{
			printf("Either Connection Closed or Error\n");
		}
		recvBuff[num] = '\0';
		printf("\nMessage from server : \n\t%s\n", recvBuff);
		printf("Do you want to continue purchasing (y/n) : ");
		scanf("%c", &cont);
		close(sockfd);
	}

	return 0;
}
