#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h>
#include <iostream>
using namespace std;
#define PORT 8080

int Flag_fin=0; 
int input[2];
int n=0;
int sum=0;
int local_time = rand()%24+1;

void * socketThread(void *arg)
{
	int new_socket = *((int *)arg);
	int berk;
	char buffer[1024] = {0};
	char hello[1024];
	string s;
	int valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer); 
	//convert string sent from client into integer and store in an array
	input[n]=stoi(buffer);
	n++;
	cout<<"Time sent by Client"<<n<<" is "<<buffer<<"\n";
	//setting a limit of max of 2 clients can connect to the server therefore 2 clocks send time to coordinator	
	if (n==2)
	{
		for(int i=0;i<n;i++)
		{
			int diff = input[i] - local_time; 
			sum=sum+diff;
		}
		cout<<sum<<" is the berkley sum"<<endl;
		berk=sum/(n+1);
		cout<<berk<<" is the average"<<endl;
		local_time=local_time+berk;
	Flag_fin=1;
	}
	while(Flag_fin!=1);
		s = to_string(local_time);
		strcpy(hello,s.c_str()); 
		write(new_socket , hello , strlen(hello));
		cout<<"Updated localtime sent to client"<<endl;
	pthread_exit(NULL);
}
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
    pthread_t tid[100];
	
	//generating random time for server
	srand(time(0)); 
	cout<<local_time<<" is the local time"<<endl;
    
	// Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    
	address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Binding the socket
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
	
	//Listen for connections
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
	int i=0;
	
	//Accepting multiple clients and creating threads for each one of them
	while(1)
	{ 
    	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    	{ 
    	    perror("accept"); 
    	    exit(EXIT_FAILURE); 
    	}
		if(pthread_create (&tid[i],NULL,socketThread,&new_socket)!=0)
		{
			cout<<"Failed to create new thread\n";
		}
    
	}
	
    return 0; 
} 
