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
char server_ip[ip_size]="192.168.10.100";

struct Router{
	char ip_addr[ip_size];
	char nexthop_addr[ip_size];
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
	//printf("Connection stablished\n");

}

vector<Router>routers;
int  inrouter(char *check_ip){
	for(int i=0;i<routers.size();i++){
		if(routers[i].match(check_ip)){
			return i;
		}
	}
	return -1;

}
void show_routing_table(){
	printf("Destination    Next Hop       Cost\n");
	printf("-----------   -----------  ...........\n");
	for(int i=0;i<routers.size();i++){
		printf("%s  %s     %d\n",routers[i].ip_addr,routers[i].nexthop_addr,routers[i].cost);
	}
}
int main(int argc, char *argv[]){

	
	char buffer[bufsz];
	FILE *topo;
	char delim[]=" ";
	

	if(argc < 3){
		printf("Wrong input format.\n Right format : %s <ip address> topo.txt\n", argv[0]);
		exit(1);
	}
	Router Cur(argv[1],0);
	Router Server(server_ip,inf);
	setup_own(Cur);
	topo=fopen(argv[2],"r");
	if(topo==NULL){
		printf("File can not open : %s\n",argv[2]);
		exit(1);
	}
	
	while(fgets(buffer,bufsz,topo)!=NULL){
		char *ptr=strtok(buffer,delim);
		char *src;
		char *dest;
		int cost;
		int i=0;
		while(ptr!=NULL){
			if(i==0) src=ptr;
			else if(i==1) dest=ptr;
			else cost=atoi(ptr);
			i++;
			ptr=strtok(NULL,delim);
		}
		if(strcmp(argv[1],src)==0){
			int x=inrouter(dest);
			if(x==-1){
				Router new_router(dest,cost);
				strcpy(new_router.nexthop_addr,dest);
				routers.push_back(new_router);
			}else{
				routers[x].cost=cost;
				strcpy(routers[x].nexthop_addr,dest);
			}
			
		}else if(strcmp(argv[1],dest)==0){
			int x=inrouter(src);
			if(x==-1){
				Router new_router(src,cost);
				strcpy(new_router.nexthop_addr,src);
				routers.push_back(new_router);
			}else{
				routers[x].cost=cost;
				strcpy(routers[x].nexthop_addr,src);
			}

		}else{
			int x=inrouter(src);
			if(x==-1){
				Router new_router(src,inf);
				strcpy(new_router.nexthop_addr, "  -------   ");
				routers.push_back(new_router);
			}
			x=inrouter(dest);
			if(x==-1){
				Router new_router(dest,inf);
				strcpy(new_router.nexthop_addr, "  -------  ");
				routers.push_back(new_router);
			}

		}

	}
	show_routing_table();
	fclose(topo);
	
	

	/*while(true){
		gets(buffer);
		if(!strcmp(buffer, "shutdown")) break;
		sendto(sockfd, buffer, 1024, 0, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
	}
	*/

	close(sockfd);

	return 0;

}
