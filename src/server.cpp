#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
int main(){
	int server_fd;   //what is server fd why such a name what is fd in it
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	if(server_fd<0){
	std::cout<<"socket creation failed \n";  //why is \n better than endl
	return 1;
	}
	std::cout<<"socket created \n";
	close (server_fd);
	return 0;
}
