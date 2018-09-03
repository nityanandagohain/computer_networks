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
struct customer{
	char ip[15];
	int port;
	struct customer *next;
};

struct fruit{
	int qty;
	char t_stamp[30];
	int unique_clients;
};

typedef struct customer customer;
typedef struct fruit fruit;

void insert_customer(customer **head,char* ip,int port,fruit* f){
	//checking if the new ip is unique
	customer *t0 = *head;
	int flag=0;
	while(t0!=NULL){
		if(strcmp(ip,t0->ip)==0 && t0->port==port){
			flag=1;
			break;
		}
		t0=t0->next;
	}
	if(!flag)
		f->unique_clients++;

	//inserting the ip and port to the linked list
	customer *t1 = (customer *)malloc(sizeof(customer));
	strcpy(t1->ip,ip);
	t1->port=port;
	t1->next = *head;
	*head = t1;
}
void setDefault(fruit* f){
	f->qty=30;
	f->unique_clients=0;
	strcpy(f->t_stamp,"");
}

void display_customers(customer *head){
	printf("Customers who have done transactions till now\n");
	while(head!=NULL){
		printf("IP: %s PORT: %d\n",head->ip,head->port);
		head=head->next;
	}
	printf("\n");
}

int main(void)
{
	int listenfd = 0,connfd = 0,n=0,num=0,sav=0; 
	struct sockaddr_in serv_addr;
	struct sockaddr_in serv_storage;
	socklen_t addr_size;

	char sendBuff[1025];
	char recvBuff[1024];
	char tempBuff[1024];
	int numrv;  
	fruit mango, orange, guava, lemon;
	setDefault(&mango);
	setDefault(&orange);
	setDefault(&guava);
	setDefault(&lemon);
	char new1[50];
	int new,i,j,l;
	struct customer *x=NULL;
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
	printf("\nProduct\tQuantity\tLast Transaction");
	printf("\nMango\t%d", mango.qty);
	printf("\nOrange\t%d",orange.qty);
	printf("\nGuava\t%d",guava.qty);
	printf("\nLemon\t%d",lemon.qty);

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
		inet_ntop(AF_INET, &ID, str2, 8);
		//insert_customer(&x,str,serv_storage.sin_port);


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
		//seperating the fruit name and the amount
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
		//converting the amount to number and storing in new
		new=atoi(new1);

		int f=0;
		if(strcmp(recvBuff,"Mango")==0)
		{
			sav=mango.qty;
			mango.qty=mango.qty-new;
			if(mango.qty<0)
			{
				mango.qty=sav;
				printf("Requested quantity of Mango is not available");
			}else{
				f=1;
				insert_customer(&x,str,serv_storage.sin_port,&mango);
				gettimestamp(mango.t_stamp);
				sprintf(tempBuff,"%d",mango.unique_clients);
			}
		}
		else if(strcmp(recvBuff,"Orange")==0)
		{
			sav=orange.qty;
			orange.qty=orange.qty-new;
			if(orange.qty<0)
			{
				orange.qty=sav;
				printf("Requested quantity of Orange is not available");
			}else{
				f=1;
				insert_customer(&x,str,serv_storage.sin_port,&orange);
				gettimestamp(orange.t_stamp);
				sprintf(tempBuff,"%d",orange.unique_clients);
			}
		}
		else if(strcmp(recvBuff,"Guava")==0)
		{
			sav=guava.qty;
			guava.qty=guava.qty-new;
			if(guava.qty<0)
			{
				guava.qty=sav;
				printf("Requested quantity of Guava is not available");
			}else{
				f=1;
				insert_customer(&x,str,serv_storage.sin_port,&guava);
				gettimestamp(guava.t_stamp);
				sprintf(tempBuff,"%d",guava.unique_clients);
			}
		}
		else if(strcmp(recvBuff,"Lemon")==0)
		{
			sav=lemon.qty;
			lemon.qty=lemon.qty-new;
			if(lemon.qty<0)
			{
				lemon.qty=sav;
				printf("Requested quantity of Lemon is not available");
			}else{
				f=1;
				insert_customer(&x,str,serv_storage.sin_port,&lemon);
				gettimestamp(lemon.t_stamp);
				sprintf(tempBuff,"%d",lemon.unique_clients);
			}
		}

		printf("\n");
		if(f){
			display_customers(x);
			strcpy(sendBuff,"No of unique clients : ");
			strcat(sendBuff,tempBuff);
		}else{
			strcpy(sendBuff,"Transaction Error");
		}
		//printf("The number of unique customers till now: %d\n",unique_customers);

		//converting unique_customers to string for sending back to client

		printf("\nMango\t%d\t%s\nOrange\t%d\t%s\nGuava\t%d\t%s\nLemon\t%d\t%s\n",mango.qty,mango.t_stamp,orange.qty,orange.t_stamp,guava.qty,guava.t_stamp,lemon.qty,lemon.t_stamp);
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

