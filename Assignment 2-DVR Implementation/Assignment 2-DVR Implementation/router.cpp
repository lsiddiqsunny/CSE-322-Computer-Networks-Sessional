#include <bits/stdc++.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define ip_size 36
#define bufsz 1024
#define inf 1000000

using namespace std;
struct sockaddr_in target_address;
struct sockaddr_in own_address;
int sockfd;
int bind_flag;
void setup_target(char *target_ip){

	target_address.sin_family = AF_INET;
	target_address.sin_port = htons(4747);
	target_address.sin_addr.s_addr = inet_addr(target_ip);

}

void setup_own(char *own_ip){
	
	own_address.sin_family = AF_INET;
	own_address.sin_port = htons(4747);
	own_address.sin_addr.s_addr = inet_addr(own_ip);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bind_flag = bind(sockfd, (struct sockaddr*) &own_address, sizeof(sockaddr_in));
	printf("Connection stablished\n");

}
struct Router{
	char ip_addr[ip_size];
	int cost;
	Router(char *ip,int c){
		strcpy(ip_addr,ip);
		cost=c;	
		//printf("%s\n",ip_addr);
	}
	bool match(char *ano_ip){
		return !strcmp(ip_addr,ano_ip);
	}

};

struct Routing_Table{
	

};
int main(int argc, char *argv[]){

	
	char buffer[bufsz];
	

	if(argc < 2){
		printf("Wrong input format.\n Right format : %s <ip address> topo.txt\n", argv[0]);
		exit(1);
	}
	setup_own(argv[1]);
	Router router1("192.168.10.2",5);
	
	

	/*while(true){
		gets(buffer);
		if(!strcmp(buffer, "shutdown")) break;
		sendto(sockfd, buffer, 1024, 0, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
	}
	*/

	close(sockfd);

	return 0;

}
