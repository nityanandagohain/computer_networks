#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
	int c_socket;
	char buffer[1024];
	struct sockaddr_in s_address, c_address;
	socklen_t addr_size;

	//socket(Internat domain, stream socket, default)
	c_socket = socket(PF_INET, SOCK_STREAM, 0);


	//server address family = internet
	s_address.sin_family = AF_INET;

	//setting port number
	s_address.sin_port = htons(16022);

	//setting ip address
	s_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	//set all bits of the padding field to 0
	memset(s_address.sin_zero, '\0', sizeof s_address.sin_zero);

	//connect to the socket server
	connect(c_socket, (struct sockaddr *) &s_address, sizeof(s_address));

	//send messages to server
	while(1){
		char message[100];
		scanf("%s",message);
		strcpy(buffer,message);
		send(c_socket, buffer, 1024, 0);

		recv(c_socket, buffer, 1024, 0);

		printf("\nMessage Recieved: %s\n",buffer);
	}
	return 0;
}
