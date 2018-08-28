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
	int sockfd = 0,n = 0,num=0;
	char recvBuff[1024];
	char sendBuff[1025];
	struct sockaddr_in serv_addr;
	memset(recvBuff, '0' ,sizeof(recvBuff));
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);//assign the port number
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//assign a the ip address of server

	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
	{
		printf("\n Error : Connect Failed \n");
		return 1;
	}
	//after connection write the data to server
	printf("Welcome to our shop\n");
	printf("Please enter the name of item you want to buy followed by the quantity eg: Mango 10\n ");
	char name[10],qty[5];
	scanf("%s %s",name,qty);
	strcat(name,"|");
	strcat(name,qty);
	strcpy(sendBuff,name); //Buffer will be eg: Mango|10
	//printf("Sending %s",sendBuff);
	if ((send(sockfd,sendBuff,strlen(sendBuff),0))== -1) 
	{
		fprintf(stderr, "Failure Sending Message\n");
		close(sockfd);
	}

	//printf("Server:Msg being sent: %s\nNumber of bytes sent: %d\n",sendBuff, strlen(sendBuff));
	//read the data send by server
	num = recv(sockfd, recvBuff, sizeof(recvBuff),0);
	if ( num <= 0 )
	{
		printf("Either Connection Closed or Error\n");

	}

	recvBuff[num] = '\0';
	printf("\nNo of unique clients from server :  %s\n",recvBuff);

	close(sockfd); 
	return 0;
}

