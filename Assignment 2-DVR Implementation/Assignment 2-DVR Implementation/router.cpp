#include <bits/stdc++.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define ip_size 36
#define bufsz 1024
#define inf 1000000
#define MAXRUT 100

using namespace std;

struct sockaddr_in own_address;
int sockfd;
int bind_flag;

struct Router{
	char ip_addr[ip_size];
	int cost;
	struct sockaddr_in address;
	int up;
	Router(char *ip,int c){
		strcpy(ip_addr,ip);
		cost=c;	
		up=1;
		address.sin_family = AF_INET;
		address.sin_port = htons(4747);
		address.sin_addr.s_addr = inet_addr(ip);
		

	}
	bool match(char *ano_ip){
		return !strcmp(ip_addr,ano_ip);
	}

};
void setup_own(Router cur){
	
	

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bind_flag = bind(sockfd, (struct sockaddr*) &cur.address, sizeof(sockaddr_in));
	printf("Connection stablished\n");

}

struct Routing_Table{
	Router router[MAXRUT];
	
	

};
int main(int argc, char *argv[]){

	
	char buffer[bufsz];
	

	if(argc < 2){
		printf("Wrong input format.\n Right format : %s <ip address> topo.txt\n", argv[0]);
		exit(1);
	}
	Router Cur(argv[1],0);
	setup_own(Cur);
	
	

	/*while(true){
		gets(buffer);
		if(!strcmp(buffer, "shutdown")) break;
		sendto(sockfd, buffer, 1024, 0, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
	}
	*/

	close(sockfd);

	return 0;

}
