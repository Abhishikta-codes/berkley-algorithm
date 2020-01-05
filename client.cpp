#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <iostream>
using namespace std; 
#define PORT 8080 
   
int main(int argc, char const *argv[]) 
{
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
   	char hello[1024];
	string s;

	//Generating random local time
	srand(time(0)); 
	int time_sent = rand()%24+1;
    cout<<"The  local time is "<<time_sent<<endl;

	s = to_string(time_sent);
	strcpy(hello,s.c_str()); 
    char buffer[1024] = {0}; 

	//Creating a socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
	//Connecting to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
	
	//Sending time to server
    send(sock , hello , strlen(hello) , 0 ); 
    printf("Time sent\n"); 
	
	//Saving the Berkley Time from server as new_time and telling the time difference between new and old time	
	while(1)
	{
		while(valread = read( sock , buffer, 1024) > 0)
		{ 
    		printf("%s\n",buffer );
			int new_time = stoi(buffer);
			cout<<"The old time and new time are "<<time_sent<<" "<<new_time<<endl;
			cout<<"The time difference is "<<new_time-time_sent<<endl;
			return 0;
		}
	}
    return 0; 
} 
