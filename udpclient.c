#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pwd.h>
#include <netinet/udp.h>

//function is used to exit program and print an error message
static void die(const char* s) { perror(s); exit(1);}

int main(int argc, char* argv[]){

    //declares the default port and hostname
    int defaultPort = 1212;
    char* hostName = "localhost";
    
    //updates the port and hostname if the user provides them
    if(argc == 2){
        hostName = argv[1];
    }else if(argc == 3){
        hostName = argv[1];
        defaultPort = atoi(argv[2]);   
    }

    if(argc > 3){
        die("Usage: client <hostname> <port>\n");
    }

    // creates a socket based on UDP
    int sock;
    if((sock = socket(AF_INET, SOCK_DGRAM, 0))<0){
        die("socket failed\n");
    }

    //constructs a server address structure
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(defaultPort);

    struct hostent *ptrh; //pointer to a host table entry
    ptrh = gethostbyname(hostName);
    if(((char*)ptrh)==NULL){
        die("invalid host\n");
    }
    memcpy(&servaddr.sin_addr, ptrh->h_addr_list[0], ptrh->h_length);

    //initializes variables name and guess that will be used for user input
    char name[100];
    int guess;

    //client begins to prompt user for name and guess
    printf("What is Your Name?\n");
    scanf("%s",name);
    printf("Pick a number 1-100\n");
    scanf("%d",&guess);
    
    char send_msg[5000], recv_msg[5000];
    sprintf(send_msg, "%s %d", name, guess); //concactenates the name and guess into one string
   
    
    
   while(1){
    sendto(sock,send_msg,strlen(send_msg),0,(struct sockaddr *)&servaddr, sizeof(servaddr));//sends the message to the server
    int bytes=recvfrom(sock,recv_msg,sizeof(recv_msg),0, NULL, NULL);//receives the message from the server
   
    //disconnects server if no bytes are received
    if (bytes <= 0) {
        die("Server disconnected");
    }
    recv_msg[bytes] = '\0'; // null-terminate the received message

    //checks if server response matches assumed responses
    //if theres a match, the client will respond with a new guess
    if(strcmp(recv_msg, "Server: High")==0){
        printf("Server: High\n");
        printf("Pick a number 1-100\n");
        scanf("%d",&guess);
        sprintf(send_msg, "%d", guess);
    }else if(strcmp(recv_msg, "Server: Low")==0){
        printf("Server: Low\n");
        printf("Pick a number 1-100\n");
        scanf("%d",&guess);
        sprintf(send_msg, "%d", guess);
    }else{
        printf("%s\n",recv_msg);//prints correct message response
        close(sock);
        break;
    }

   }
    return 0;
}