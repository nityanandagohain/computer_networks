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
struct customer{
	char ip[15];
	int port;
	struct customer *next;
};
typedef struct customer customer;

void insert_customer(customer **head,char* ip,int port){
	customer *t1 = (customer *)malloc(sizeof(customer));
	strcpy(t1->ip,ip);
	t1->port=port;
	t1->next = *head;
	*head = t1;
}

void display_customers(customer *head){
	printf("Customers who have done transactions till now");
	while(head!=NULL){
		printf("IP: %s PORT: %d\n",head->ip,head->port);
		head=head->next;
	}
	printf("\n");
}

int main(void)
{
	int listenfd = 0,connfd = 0,n=0,num=0; 
	struct sockaddr_in serv_addr;
	struct sockaddr_in serv_storage;
	socklen_t addr_size;

	char sendBuff[1025];
	char recvBuff[1024];  
	int numrv;  
	int mango=30, orange=30, guava=30, petrol=30 ,sav;
	char new1[50];
	int new,i,j,l;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket retrieve success\n");

	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;    
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serv_addr.sin_port = htons(5000);    

	bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

	if(listen(listenfd, 10) == -1){
		printf("Failed to listen\n");
		return -1;
	}
	printf("\nAvailable items");
	printf("\nProduct\tQuantity");
	printf("\nMango\t%d", mango);
	printf("\nOrange\t%d",orange);
	printf("\nGuava\t%d",guava);
	printf("\nPetrol\t%d",petrol);

	while(1)
	{
		addr_size = sizeof serv_storage;
		connfd = accept(listenfd, (struct sockaddr*)&serv_storage,&addr_size);	  // accept awaiting request

		//identify the client
		struct sockaddr_in* cliIP = (struct sockaddr_in*)&serv_storage;
		struct in_addr ipAddr = cliIP->sin_addr;
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);
		char* ID = cliIP->sin_zero;
		char str2[8];
		struct customer *x=NULL;
		inet_ntop(AF_INET, &ID, str2, 8);
		insert_customer(&x,str,serv_storage.sin_port);
		//strcpy(x.ip,str);
		//x.port = serv_storage.sin_port;
		//printf("\nClient IP is: %s", x.ip);
		//printf("\nClient port is: %d", x.port);


		//read the data send by client
		num = recv(connfd, recvBuff, sizeof(recvBuff),0);
		if ( num <= 0 )
		{
			printf("Either Connection Closed or Error\n");

		}

		recvBuff[num] = '\0';
		i=0;
		j=0;
		l=strlen(recvBuff);
		while(i<l){
			if(recvBuff[i]=='|'){
				recvBuff[i]='\0';
				i++;
				j=0;
				while(i<l){
					new1[j]=recvBuff[i];
					j++;
					i++;
				}
				new1[j]='\0';
			}
			i++;
		}
		new=atoi(new1);
		if(strcmp(recvBuff,"Mango")==0)
		{
			sav=mango;
			mango=mango-new;
			if(mango<0)
			{
				mango=sav;
				printf("Requested quantity of Mango is not available");
			}
		}
		else if(strcmp(recvBuff,"Orange")==0)
		{
			sav=orange;
			orange=orange-new;
			if(orange<0)
			{
				orange=sav;
				printf("Requested quantity of Orange is not available");
			}
		}
		else if(strcmp(recvBuff,"Guava")==0)
		{
			sav=guava;
			guava=guava-new;
			if(guava<0)
			{
				guava=sav;
				printf("Requested quantity of Guava is not available");
			}
		}
		else if(strcmp(recvBuff,"Petrol")==0)
		{
			sav=petrol;
			petrol=petrol-new;
			if(petrol<0)
			{
				petrol=sav;
				printf("Requested quantity of Petrol is not available");
			}
		}

		
		display_customers(x);

		//write the data for client   
		//fgets(sendBuff,1025,stdin);
		printf("\nMango\t%d\nOrange\t%d\nGuava\t%d\nPetrol\t%d\n",mango,orange,guava,petrol);
		if ((send(connfd,sendBuff,strlen(sendBuff),0))== -1) 
		{
			fprintf(stderr, "Failure Sending Message\n");
			close(connfd);
			break;
		}

		// printf("Server:Msg being sent: %s\nNumber of bytes sent: %d\n",sendBuff, strlen(sendBuff));


		close(connfd); //Close Connection Socket   
		sleep(1);
	} //End of Inner While...


	return 0;
}

