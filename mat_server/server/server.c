#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

void gettimestamp(char arr[]){
	time_t ltime; //calendar time
	ltime = time(NULL); //current calendar time
	strcpy(arr,asctime(localtime(&ltime)));
}

struct user{
	char ip[15];
	int port;
	struct user *next;
};

typedef struct user user;

void insert_user(user **head,char* ip,int port){
	//checking if the new ip is unique
	user *t0 = *head;
	int flag=0;
	while(t0!=NULL){
		if(strcmp(ip,t0->ip)==0 && t0->port==port){
			flag=1;
			break;
		}
		t0=t0->next;
	}

	//inserting the ip and port to the linked list
	user *t1 = (user *)malloc(sizeof(user));
	strcpy(t1->ip,ip);
	t1->port=port;
	t1->next = *head;
	*head = t1;
}

void display_users(user *head){
	printf("Customers who have done transactions till now\n");
	while(head!=NULL){
		printf("IP: %s PORT: %d\n",head->ip,head->port);
		head=head->next;
	}
	printf("\n");
}

int  parse_and_return_result(char *str){
	float num1,num2;
	char op,operand1[50],operand2[50];
	int flag = 0; //0 = 1st operand, 1= operator, 2 = 2nd operand
	int len = strlen(str);
	int i,j=0,k=0;
	for(i=0;i<len;i++){
		if(str[i]==' '){
			flag ++;
			while((str[i+1]==' ')){
				i++;
			}
		}else if(flag == 0 ){
			operand1[j++]=str[i];	
		}else if(flag == 1){
			op = str[i];
		}else if( flag == 2){
			operand2[k++]=str[i];
		}
	}
	if(!(flag >= 2)){
		return -1;
	}
	operand1[j]=operand2[k]='\0';
	num1 = atoi(operand1);
	num2 = atoi(operand2);
	printf("No's are %f %f", num1, num2);
	switch(op){
		case '+':
			return num1 + num2;
		case '-':
			return num1 - num2;
		case '*':
			return num1 * num2;
		case '/':
			return num1 / num2;
		default:
			return -1;
	}
	return 0;
}


int main(void)
{
	int listenfd = 0,connfd = 0,n=0,num=0,sav=0; 
	struct sockaddr_in serv_addr;
	struct sockaddr_in serv_storage;
	socklen_t addr_size;

	int new,i,j,l;
	struct user *x=NULL;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket retrieve success\n");

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;    
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serv_addr.sin_port = htons(5000);    

	bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

	if(listen(listenfd, 10) == -1){
		printf("Failed to listen\n");
		return -1;
	}

	printf("Server running\n");
	//parse_and_return_result("1 + 2");

	while(1)
	{
		char sendBuff[1025];
		char recvBuff[1024];
		memset(sendBuff, '0', sizeof(sendBuff));
		printf("\nSTART\n");
		addr_size = sizeof serv_storage;
		connfd = accept(listenfd, (struct sockaddr*)&serv_storage,&addr_size);

		//identify the client
		struct sockaddr_in* cliIP = (struct sockaddr_in*)&serv_storage;
		struct in_addr ipAddr = cliIP->sin_addr;
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);
		char* ID = cliIP->sin_zero;
		char str2[8];
		inet_ntop(AF_INET, &ID, str2, 8);

		//read the data send by client
		num = recv(connfd, recvBuff, sizeof(recvBuff),0);
		if ( num <= 0 )
		{
			printf("Either Connection Closed or Error\n");
			continue;

		}
		printf("recieved : %s",recvBuff);

		//Parsing and getting result
		int result = parse_and_return_result(recvBuff);
		if(result == -1 ){
			printf("Error\n");
			strcpy(sendBuff,"Error");
		}else{
			printf("Successfullt computed\n");
			sprintf(sendBuff,"Output: %d",result);
		}

		//sending the result to the client
		if ((send(connfd,sendBuff,strlen(sendBuff),0))== -1) 
		{
			fprintf(stderr, "Failure Sending Message\n");
			close(connfd);
			break;
		}
		close(connfd); 
	}
	return 0;
}

