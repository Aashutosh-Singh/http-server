#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<cstring>
using namespace std;
int main(){
	int server_fd=socket(AF_INET,SOCK_STREAM,0);
	if(server_fd<0){
		perror("socket connection failed");
		return 1;
	}
	
	cout<<"socket created \n";
	sockaddr_in server_addr;
	memset(&server_addr,0,sizeof(server_addr));

	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=INADDR_ANY;
	server_addr.sin_port=htons(8080);
	if(bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr))<0){
		perror("Bind failed");
		return 1;
	}

	cout<<"Bind sucessful to port 8080\n";

	if(listen(server_fd,1200)<1){
		perror("Listen fialed ");
		return 1;
	}
	cout<<"Listening on 8080\n";
	sockaddr_in client_address;
	socklen_t client_len=sizeof(client_address);
	int client_fd=accept(server_fd,(sockaddr*)&client_address,&client_len);
	if(client_fd<0){
		perror("Acceptance failed");
		return 1;
	}
	cout<<"Client connected\n";
	close(client_fd);

	close (server_fd);
	return 0;
}
