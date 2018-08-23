#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
	int welcome_socket,new_socket;
	char buffer[1024];
	struct sockaddr_in s_address;
	struct sockaddr_in s_storage;
	socklen_t addr_size;

	welcome_socket = socket(PF_INET, SOCK_STREAM, 0);
	s_address.sin_family = AF_INET;
	s_address.sin_port = htons(16022);
	s_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	//setting all padding field to 0
	memset(s_address.sin_zero, '\0', sizeof s_address.sin_zero);

	//binding the address to socket
	bind(welcome_socket, (struct sockaddr *) &s_address, sizeof(s_address));


	//Listen to 5 client at max
	if(listen(welcome_socket, 5) ==0){
		printf("Listening\n");
	}else{
		printf("Error\n");
	}

	while(1){
		addr_size = sizeof s_storage;
		new_socket = accept(welcome_socket, (struct sockaddr *) &s_storage, & addr_size);

		struct sockaddr_in *cliIP = (struct sockaddr_in *)&s_storage;
		struct in_addr ipAddr = cliIP->sin_addr;
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ipAddr, str, INET_ADDRSTRLEN);
		char *ID = cliIP->sin_zero;
		char str2[0];
		inet_ntop(AF_INET, &ID, str2, 0);

		int i;
		for(i=0;i<8;i++){
			printf("%c",s_storage.sin_zero[i]);
		}

		if(strcmp(buffer,"hi")==0){
			send(new_socket,"hello",23,0);
		}else{
			send(new_socket,"send message",23,0);
		}

		while(1){
			recv(new_socket, buffer, 1024, 0);

			if(strcmp(buffer,"hi")==0){
				send(new_socket,"hello",23,0);
			}else{
				send(new_socket,"send message",23,0);
			}
			printf("-----");
		}
	}
	return 0;
}
